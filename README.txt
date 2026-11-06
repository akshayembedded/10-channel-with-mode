STM8S003F3P / 10-CHANNEL CHASER
35 patterns adapted from the current 22-channel project. No PWM.
Pins OUT1..10: PC3 PC4 PC5 PC6 PC7 PA1 PA2 PA3 PD2 PD3.
POT PD6/AIN6; button PD4 to ground (internal pull-up); preserve PD1/SWIM.
Button, TIM4 ADC sampling and EEPROM boot cycling controlled by config.h.
EEPROM: next boot mode at 0x4000, valid range 0x4000..0x407F.
Boots alternate playlist / steady all-on; button toggles modes live.
Randomly shuffled fixed playlist is in PLAYLIST.txt. Four long patterns run
once each; all others three times, retaining the agreed repetition settings.
Split odd fill: 1,7,3,9,5 -> all OFF; even fill: 6,2,8,4,10 -> all OFF.
Half-row strobe: first 5 LEDs five flashes, then last 5 five flashes.
Strobe ON times are direct program constants; OFF/movement follows POT.
Open LED_Studio.html offline for matching 10-LED simulation and editing.
Rebuild studio with: node tools/build_studio.js
Host checks: node tools/verify_10_channel.js; node tools/verify_controls.js
Open 10_channel_eeprom.stp in STVD/Cosmic and use Release / Rebuild All.
8 KB Flash, 1 KB RAM, 128 B EEPROM. Target compilation/link map must
confirm all patterns fit before programming; old binaries are outdated.
Cosmic is unavailable here; host tests do not replace the target build.

The playlist is a fixed shuffle; each complete cycle uses the same order.

LATEST COUNTS
Odd x10 / Even x1 Blink removed. Even x10 / Odd x10 Blink added.
Separate half-row 5-strobe and 10-strobe patterns use LEDs 1..5 then 6..10.
All LEDs Strobe runs 10 times per playlist. Forward Three Strobe Fill
uses 3 flashes per new LED and retains one playlist repetition.
Total 35 patterns; the shuffled order otherwise remains unchanged.
