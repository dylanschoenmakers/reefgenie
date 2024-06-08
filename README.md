# Reef Genie

Please note: this is currently a work in progress

Reef Genie is an alkalinity, calcium and magnesium water tester for reef tank aquariums. It's designed to interface with Home Assistant through ESPHome.


## Part list

Below is an initial BOM that contain all parts and estimated prices to build this water tester yourself.


| Item                            | Qty |  Price     |  Total          |
| ------------------------------- | --- | ---------- | --------------- |
| ESP32 board                     | 1   |  €  10.00  |  €       10.00  |
| Nema 17 stepper                 | 2   |  €  12.00  |  €       24.00  |
| Peristaltic Pump Head           | 1   |  €    9.00 |  €         9.00 |
| Silicon tubing 2mm ID x 4 mm OD | 1   |  €    7.00 |  €         7.00 |
| TMC2208 motor driver            | 2   |  €    3.00 |  €         6.00 |
| DRV88                           | 1   |  €    3.00 |  €         3.00 |
| TCS34725 Colorimeter            | 1   |  €    5.00 |  €         5.00 |
| Wiring                          | 1   |  €    3.00 |  €         3.00 |
| Bolts and nuts                  | 1   |  €    5.00 |  €         5.00 |
| ASA Filament                    |     |            |  €       15.00  |
| PCTG Filament                   |     |            |  €       15.00  |
| *Grand total*                   |     |            |  €  102.00      |


## Tritation procedures

Reef Genie embeds tritation procedures for testing Alkalinity, Calcium, Magnesium, Nitrate, Phosphate, and possibly others. Each procedure has a set of steps that are executed in sequence that can be 

To illustrate a calcium procedure:
1. Select tank water valve
1. Add 2ml of tank water to the reactor vial
1. Select reagent A valve
1. Add 2 drops of reagent A
2. Select reagent B valve
2. Add 2 drops of reagent B
4. Stir the solution for 2 seconds
5. Test the color of the solution with the colorimeter
6. If not blue, repeat last 3 steps
7. Record the number of drops of reagent B used
8. Calculate the calcium concentration
9. Select waste water valve
10. Drain the reactor vial
9. Select tank water valve
9. Add 1ml of tank water to the reactor vial to clean
9. Select waste water valve
10. Drain the reactor vial