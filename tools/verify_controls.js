const fs=require('fs'),vm=require('vm'),assert=require('assert/strict'),path=require('path');
const root=path.join(__dirname,'..');
const adc=fs.readFileSync(path.join(root,'adc.c'),'utf8');
const main=fs.readFileSync(path.join(root,'main.c'),'utf8');
function preprocess(s,defs){
 let active=true,stack=[];return s.split(/\r?\n/).filter(line=>{
 let m=line.match(/^#if (.*)/);if(m){stack.push({parent:active,test:!!Function(...Object.keys(defs),'return '+m[1])(...Object.values(defs))});active=active&&stack.at(-1).test;return false;}
 if(/^#else/.test(line)){active=stack.at(-1).parent&&!stack.at(-1).test;return false;}
 if(/^#endif/.test(line)){active=stack.pop().parent;return false;}
 return active&&!/^#include/.test(line);
 }).join('\n');
}
function translate(s){return s.replace(/#asm[\s\S]*?#endasm/g,'').replace(/@far @interrupt /g,'').replace(/\b(?:unsigned char|unsigned int|int|void) (\w+)\([^)]*\)/g,(x,n)=>'function '+n+'()').replace(/\b(?:static\s+)?(?:volatile\s+)?(?:unsigned int|unsigned char|int) (\w+)/g,'var $1').replace(/\((?:unsigned char|unsigned int|unsigned long|int)\)/g,'').replace(/\b(\d+)(?:UL|U)\b/g,'$1');}
for(const button of [0,1])for(const timer of [0,1]){
 const code=translate(preprocess(adc,{ENABLE_MODE_BUTTON:button,ENABLE_TIMER_ADC:timer}));
 const env={PD_IDR:0x10,ADC_CR1:0,ADC_CSR:0,ADC_DRL:0,ADC_DRH:0,PD_CR2:0,PD_DDR:0xff,PD_CR1:0,EXTI_CR1:0x3f,CLK_PCKENR1:0,TIM4_CR1:0,TIM4_IER:0,TIM4_PSCR:0,TIM4_ARR:0,TIM4_CNTR:0,TIM4_EGR:0,TIM4_SR:0};
 const c=vm.createContext(env);vm.runInContext(code,c);c.control_timer_init();
 assert.equal(c.TIM4_CR1,button||timer?1:0);assert.equal(c.PD_CR2&16,button?16:0);
 if(button)assert.equal(c.EXTI_CR1,0xbf);
 c.TIM4_UPD_OVF_IRQHandler();assert.equal(c.adc_busy,timer);
 c.ADC_CSR=0xa9;c.ADC_DRL=0xff;c.ADC_DRH=3;c.ADC_IRQHandler();assert.equal(c.adc_value,1023);assert.equal(c.adc_busy,0);assert.equal(c.adc_speed_read(),1000);
 c.adc_value=0;assert.equal(c.adc_speed_read(),10);
 c.PD_IDR=0;c.EXTI_PORTD_IRQHandler();
 for(let n=0;n<2;n++)c.TIM4_UPD_OVF_IRQHandler();assert.equal(c.button_take_press(),0);
 c.TIM4_UPD_OVF_IRQHandler();assert.equal(c.button_take_press(),button);
 for(let n=0;n<50;n++)c.TIM4_UPD_OVF_IRQHandler();assert.equal(c.button_take_press(),0);
 c.PD_IDR=16;for(let n=0;n<3;n++)c.TIM4_UPD_OVF_IRQHandler();
 c.PD_IDR=0;c.EXTI_PORTD_IRQHandler();for(let n=0;n<3;n++)c.TIM4_UPD_OVF_IRQHandler();assert.equal(c.button_take_press(),button);
}
let ee=main.slice(main.lastIndexOf('// --- EEPROM Functions ---'));
ee=ee.replace(/\*\(volatile unsigned char \*\)address/g,'EEPROM_DATA').replace(/unsigned char eeprom_write_byte\(unsigned int address, unsigned char data\)/,'function eeprom_write_byte(address,data)').replace(/unsigned char eeprom_read_byte\(unsigned int address\)/,'function eeprom_read_byte(address)').replace(/unsigned (?:int|char) (\w+);/g,'let $1;').replace(/\(unsigned char\)/g,'').replace(/\b(\d+)U\b/g,'$1');
for(const fault of ['','unlock','write','timeout','readback']){
 let data=255,status=0,key=0,writes=0;
 const env={EEPROM_START_ADDR:0x4000,EEPROM_WAIT_LIMIT:20};
 Object.defineProperties(env,{
 FLASH_DUKR:{set:v=>{if(key===0xae&&v===0x56&&fault!=='unlock')status|=8;key=v;}},
 FLASH_IAPSR:{get:()=>{let s=status;status&=~5;return s;},set:v=>{status=v;}},
 EEPROM_DATA:{get:()=>data,set:v=>{writes++;if(fault==='write')status|=1;else if(fault!=='timeout'){data=fault==='readback'?254:v;status|=4;}}}
 });
 const c=vm.createContext(env);vm.runInContext(ee,c);assert.equal(Number(c.eeprom_write_byte(0x4000,1)),fault?0:1);assert.equal(status&8,0);
 assert.equal(c.eeprom_write_byte(0x3fff,1),0);assert.equal(c.eeprom_write_byte(0x4080,1),0);
}
// Every pattern must unwind immediately after a mode change at any delay.
let patterns=main.slice(main.indexOf('void mode_forward_fill_forward_empty(void)\n{')>=0?main.indexOf('void mode_forward_fill_forward_empty(void)\n{'):main.indexOf('void mode_forward_fill_forward_empty(void)\r\n{'),main.lastIndexOf('// --- EEPROM Functions ---'));
patterns=patterns.replace(/void (\w+)\((?:void)?\)/g,'function $1()').replace(/\b(?:unsigned int|int)\s+([^;]+);/g,'let $1;').replace(/\b(0x[0-9a-fA-F]+|[0-9]+)(?:UL|U)\b/g,'$1');
const names=[...patterns.matchAll(/function (mode_\w+)\(/g)].map(m=>m[1]);let checked=0;
for(const name of names){
 let delays=0;const c=vm.createContext({mode:0,channel:10,mapped_value:505,ledon:()=>{},ledoff:()=>{},allon:()=>{},alloff:()=>{},myDelay:()=>delays++});c.led_bit=i=>i>=0&&i<10?1<<i:0;c.pattern_mask=mask=>{for(let i=0;i<10;i++){if(mask&(1<<i))c.ledon(i);else c.ledoff(i);}};vm.runInContext(patterns,c);c[name]();
 for(let stop=1;stop<=delays;stop++){
  let count=0;const pin=()=>assert.equal(c.mode,0,name+' stale output');c.ledon=pin;c.ledoff=pin;c.alloff=pin;c.allon=pin;c.mode=0;
  c.myDelay=()=>{if(++count===stop)c.mode=1;};c[name]();assert.equal(count,stop);checked++;
 }
}
console.log('PASS: all 4 button/timer combinations; ADC endpoints; debounced press/hold/release; EEPROM failure/timeout/readback/range checks; '+checked+' animation interruption points.');
