const fs = require('fs'), path = require('path');
const root = path.join(__dirname, '..');
const main = fs.readFileSync(path.join(root, 'main.c'), 'utf8');
const start = main.search(/void mode_forward_fill_forward_empty\(void\)\s*\{/);
let body = main.slice(start, main.lastIndexOf('// --- EEPROM Functions ---'));
body = body.replace(/\b(0x[0-9a-fA-F]+|[0-9]+)(?:UL|U)\b/g,'$1').replace(/void (\w+)\((?:void)?\)/g, 'function $1()').replace(/\b(?:unsigned int|int)\s+([^;]+);/g, 'let $1;');
const labels = {
  "mode_comet_ping_pong": "Comet Ping-pong",
  "mode_twin_chase_cross_through": "Twin Chase / Cross-through",
  "mode_center_out_strobe_fill": "Center-out Strobe Fill",
  "mode_dark_pair_mirror_chase": "Dark Pair Mirror Chase",
  "mode_two_ended_chase_and_stack": "Two-ended Chase-and-Stack",
  "mode_expanding_contracting_band": "Expanding / Contracting Band",
  "mode_triple_strobe_burst_pause": "Triple Strobe Burst / Pause",
  "mode_half_row_ten_strobe_alternate": "Half-row Ten-strobe Alternate",
  "mode_half_row_five_strobe_alternate": "Half-row Five-strobe Alternate",
  "mode_bouncing_dark_window": "Bouncing Dark Window",
  "mode_forward_reverse_ripple_fill": "Forward / Reverse Ripple Fill",
  "mode_forward_fill_forward_empty": "Forward Fill / Forward Empty",
  "mode_forward_single_led_chase": "Forward Single LED Chase",
  "mode_reverse_single_led_chase": "Reverse Single LED Chase",
  "mode_forward_two_led_chase": "Forward Two LED Chase",
  "mode_forward_strobe_chain_fill": "Forward Strobe Chain Fill",
  "mode_reverse_strobe_chain_fill": "Reverse Strobe Chain Fill",
  "mode_ping_pong_double_strobe_chase": "Ping-pong Double Strobe Chase",
  "mode_forward_fill_reverse_empty": "Forward Fill / Reverse Empty",
  "mode_inward_mirror_strobe_fill": "Inward Mirror Strobe Fill",
  "mode_outward_mirror_fill_inward_empty": "Outward Mirror Fill / Inward Empty",
  "mode_all_leds_strobe": "All LEDs Strobe",
  "mode_ping_pong_two_led_strobe_chase": "Ping-pong Two LED Strobe Chase",
  "mode_ping_pong_dark_chase": "Ping-pong Dark Chase",
  "mode_inward_outward_mirror_strobe_chase": "Inward Outward Mirror Strobe Chase",
  "mode_inward_outward_mirror_two_led_strobe_chase": "Inward Outward Mirror Two-LED Strobe Chase",
  "mode_inward_mirror_strobe_chase": "Inward Mirror Strobe Chase",
  "mode_outward_mirror_strobe_chase": "Outward Mirror Strobe Chase",
  "mode_ping_pong_split_pair_strobe_chase": "Ping-pong Split Pair Strobe Chase",
  "mode_ping_pong_triple_strobe_chase": "Ping-pong Triple Strobe Chase",
  "mode_ping_pong_three_led_strobe_chase": "Ping-pong Three LED Strobe Chase",
  "mode_odd_eight_blinks_then_even_eight_blinks": "Odd ×8 / Even ×8 Blink",
  "mode_forward_three_strobe_fill": "Forward Three Strobe Fill",
  "mode_split_odd_even_double_strobe_fill": "Split Odd/Even Double-strobe Fill",
  "mode_even_ten_blinks_then_odd_ten_blinks": "Even ×10 / Odd ×10 Blink",
};
const playlist = [...main.slice(main.indexOf('void main(void)'), main.indexOf('void myDelay(int l)\n{')).matchAll(/for\(i=0;i<(\d+) && mode==0;i\+\+\) (mode_\w+)\(\);/g)].map(m => ({id:m[2], repeats:+m[1], name:labels[m[2]]}));
if (playlist.length !== 35) throw Error('Expected 35 firmware playlist entries');
const engine = `const PRESETS=${JSON.stringify(playlist)};
function capturePattern(id, speed) {
 let channel=10,mode=0,mapped_value=speed,mask=0; const frames=[];
 function ledon(i){if(i<0||i>=10)throw Error('Invalid LED');mask|=1<<i;}
 function ledoff(i){if(i<0||i>=10)throw Error('Invalid LED');mask&=~(1<<i);}
 function alloff(){mask=0;} function allon(){mask=1023;}
 function led_bit(i){return i>=0&&i<10?1<<i:0;}
 function pattern_mask(nextMask){mask=nextMask;}
 function myDelay(units){frames.push({mask:mask>>>0,units});}
 ${body}
 const patterns={${playlist.map(p=>p.id).join(',')}};
 if(!patterns[id])throw Error('Unknown pattern'); patterns[id]();
 return frames;
}
`;
const template = fs.readFileSync(path.join(root, 'studio.template.html'), 'utf8');
fs.writeFileSync(path.join(root, 'LED_Studio.html'), template.replace('/* FIRMWARE_ENGINE */', () => engine).replace('/* STUDIO_SUGGESTIONS */', ''));
console.log('Built standalone LED_Studio.html with 35 current firmware patterns.');
