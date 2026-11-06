const fs=require('fs'),path=require('path'),vm=require('vm'),assert=require('assert/strict');
const root=path.join(__dirname,'..'),read=n=>fs.readFileSync(path.join(root,n),'utf8');
const main=read('main.c'),html=read('LED_Studio.html');
const script=html.match(/<script>([\s\S]*?)<\/script>/)[1];new vm.Script(script);
const engine=script.slice(0,script.indexOf('const PINS='));
const c=vm.createContext({});vm.runInContext(engine+';globalThis.presets=PRESETS;',c);
assert.equal(c.presets.length,35);assert.equal(new Set(c.presets.map(p=>p.id)).size,35);
const once=new Set(['mode_forward_strobe_chain_fill','mode_reverse_strobe_chain_fill','mode_ping_pong_triple_strobe_chase','mode_forward_three_strobe_fill']);
let frames=0;
for(const p of c.presets){
 assert.equal(p.repeats,p.id==='mode_all_leds_strobe'?10:once.has(p.id)?1:3);
 for(const speed of [10,180,1000]){
  const seq=c.capturePattern(p.id,speed);assert(seq.length>0);
  seq.forEach(f=>{assert(Number.isInteger(f.mask)&&f.mask>=0&&f.mask<=1023);assert(Number.isInteger(f.units)&&f.units>0);});frames+=seq.length;
 }
}
const capture=(id,speed=180)=>JSON.parse(JSON.stringify(c.capturePattern('mode_'+id,speed)));
assert.deepEqual(capture('forward_single_led_chase').map(f=>f.mask),Array.from({length:10},(_,i)=>1<<i));
assert.deepEqual(capture('reverse_single_led_chase').map(f=>f.mask),Array.from({length:10},(_,i)=>1<<(9-i)));
assert.equal(capture('forward_fill_forward_empty').length,20);
assert.equal(capture('forward_fill_forward_empty')[9].mask,1023);
assert.equal(capture('two_ended_chase_and_stack').at(-1).mask,1023);
assert.equal(capture('forward_reverse_ripple_fill').length,40);
for(const speed of [10,180,1000]){
 const half=capture('half_row_five_strobe_alternate',speed);assert.equal(half.length,20);
 half.forEach((f,i)=>{assert.equal(f.mask,i%2?0:i<10?31:992);assert.equal(f.units,i%2?speed:100);});
 const split=capture('split_odd_even_double_strobe_fill',speed);assert.equal(split.length,52);
 const orders=[[1,7,3,9,5],[6,2,8,4,10]];let n=0;
 for(const order of orders){let mask=0;for(const led of order){const prior=mask;mask|=1<<(led-1);assert.deepEqual(split.slice(n,n+5),[{mask,units:100},{mask:prior,units:speed},{mask,units:100},{mask:prior,units:speed},{mask,units:speed}]);n+=5;}assert.deepEqual(split[n++],{mask:0,units:speed});}
 const mirror=capture('inward_outward_mirror_two_led_strobe_chase',speed);assert.equal(mirror.length,16);
 for(let step=0;step<8;step++){const pos=step<4?step:7-step;assert.equal(mirror[step*2].mask,(1<<pos)|(1<<(pos+1))|(1<<(9-pos))|(1<<(8-pos)));assert.equal(mirror[step*2+1].mask,0);}
}
const pins=main.match(/led_pins\[channel\] = \{([^}]+)\}/)[1].match(/\d+/g).map(Number);
const ports=[...main.match(/\*ports\[channel\] = \{([^}]+)\}/)[1].matchAll(/&(P[A-Z])_ODR/g)].map(m=>m[1]);
assert.deepEqual(pins.map((b,i)=>ports[i]+b),['PC3','PC4','PC5','PC6','PC7','PA1','PA2','PA3','PD2','PD3']);
const adc=read('adc.c'),vectors=read('stm8_interrupt_vector.c');
assert(adc.includes('ADC_CSR = 0x26'));assert(adc.includes('ADC_TDRL |= 0x40'));
assert(adc.includes('(EXTI_CR1 & 0x3F) | 0x80'));assert(adc.includes('PD_IDR & 0x10'));
assert(vectors.includes('EXTI_PORTD_IRQHandler}, /* irq6 Port D */'));
assert(!/PB_|PE_|stm8s105|1024U/.test(main+adc));
const project=read('10_channel_eeprom.stp');assert.equal((project.match(/String.100.0=STM8S003F3P/g)||[]).length,2);
assert.equal((project.match(/0x4000-0x407f/g)||[]).length,2);assert.equal((project.match(/0x8080-0x9fff/g)||[]).length,2);
// Execute actual C bodies with pin checks, and ensure each returns OFF.
let body=main.slice(main.search(/void mode_forward_fill_forward_empty\(void\)\s*\{/),main.lastIndexOf('// --- EEPROM Functions ---'));
body=body.replace(/void (\w+)\((?:void)?\)/g,'function $1()').replace(/\b(?:unsigned int|int)\s+([^;]+);/g,'let $1;').replace(/\b(0x[0-9a-fA-F]+|[0-9]+)U\b/g,'$1');
let mask=0;const pin=i=>assert(Number.isInteger(i)&&i>=0&&i<10,'Invalid LED index: '+i);
const env={channel:10,mode:0,mapped_value:180,ledon:i=>{pin(i);mask|=1<<i;},ledoff:i=>{pin(i);mask&=~(1<<i);},allon:()=>mask=1023,alloff:()=>mask=0,myDelay:()=>{},led_bit:i=>i>=0&&i<10?1<<i:0,pattern_mask:m=>{assert(m>=0&&m<=1023);mask=m;}};
const run=vm.createContext(env);vm.runInContext(body,run);for(const p of c.presets){mask=0;run[p.id]();assert.equal(mask,0,p.id+' leaves outputs ON');}
console.log('PASS: 35 reordered 10-channel patterns, '+frames+' frames at three speeds; exact pin map, split orders, five-LED halves, mirror geometry, OFF cleanup and STM8S003 settings.');

assert(!c.presets.some(p=>p.id==='mode_odd_ten_blinks_then_even_once'));
assert.equal(capture('half_row_ten_strobe_alternate').length,40);
assert.equal(capture('even_ten_blinks_then_odd_ten_blinks').length,40);
assert.equal(capture('forward_three_strobe_fill').length,70);
for(let i=0;i<40;i++){const f=capture('half_row_ten_strobe_alternate')[i];assert.equal(f.mask,i%2?0:i<20?31:992);}
for(let i=0;i<40;i++){const f=capture('even_ten_blinks_then_odd_ten_blinks')[i];assert.equal(f.mask,i%2?0:i<20?682:341);}
console.log('PASS: requested removals, 5/10 half-row strobes, even/odd ten blinks, three-strobe fill and ten all-LED repeats.');
