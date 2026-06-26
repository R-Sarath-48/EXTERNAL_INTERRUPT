
# STM32F411 Bare-Metal External Interrupt (EXTI) Example

A simple **Bare-Metal STM32F411** project demonstrating how to configure an **External Interrupt (EXTI13)** using a push button connected to **PC13** to toggle an LED connected to **PA5**.

This project uses **direct register programming** without STM32 HAL or LL libraries.

---

## Objective

When the user presses the push button connected to **PC13**:

* A falling edge is detected.
* EXTI Line 13 generates an interrupt.
* NVIC receives the interrupt request.
* CPU jumps to `EXTI15_10_IRQHandler()`.
* LED connected to **PA5** toggles.

---

## Hardware

| Component   | STM32 Pin |
| ----------- | --------- |
| LED         | PA5       |
| Push Button | PC13      |

---

## Interrupt Flow

```
Button Press
      │
      ▼
PC13 Voltage Changes
      │
      ▼
GPIO Input Buffer
      │
      ▼
Schmitt Trigger
      │
      ▼
SYSCFG
(PC13 → EXTI13)
      │
      ▼
EXTI Edge Detector
(Falling Edge)
      │
      ▼
Pending Register PR13 = 1
      │
      ▼
Interrupt Mask Check
(IMR13)
      │
      ▼
NVIC
      │
      ▼
CPU Saves Context
      │
      ▼
Vector Table
      │
      ▼
EXTI15_10_IRQHandler()
```

---

## Interrupt Service Routine

```c
void EXTI15_10_IRQHandler(void)
{
    if(*EXTI_PR & (1<<13))
    {
        *EXTI_PR |= (1<<13);     // Clear pending flag

        *GPIOA_ODR ^= (1<<5);    // Toggle LED
    }
}
```

### Step-by-Step

### Step 1

Check whether EXTI13 generated the interrupt.

```c
if(*EXTI_PR & (1<<13))
```

---

### Step 2

Clear the pending flag.

```c
*EXTI_PR |= (1<<13);
```

If the flag is not cleared, the ISR will be entered repeatedly.

---

### Step 3

Toggle LED.

```c
*GPIOA_ODR ^= (1<<5);
```

---

## Complete Execution Flow

```
Power ON
    │
    ▼
main()
    │
    ▼
Button_init()
    │
    ├── Enable GPIO Clocks
    ├── Configure PA5 Output
    ├── Configure PC13 Input
    ├── Enable SYSCFG
    ├── Route PC13 → EXTI13
    ├── Enable Falling Edge
    ├── Unmask EXTI13
    └── Enable NVIC
            │
            ▼
while(1)
            │
            ▼
User Presses Button
            │
            ▼
PC13 Goes HIGH → LOW
            │
            ▼
EXTI13 Detects Falling Edge
            │
            ▼
PR13 = 1
            │
            ▼
NVIC Interrupt
            │
            ▼
CPU Automatically Saves

R0
R1
R2
R3
R12
LR
PC
xPSR

            │
            ▼
EXTI15_10_IRQHandler()
            │
            ▼
Clear PR13
            │
            ▼
Toggle PA5 LED
            │
            ▼
ISR Ends
            │
            ▼
CPU Restores Context
            │
            ▼
Returns to while(1)
```

---

## Memory-Mapped Registers Used

| Register       | Address      | Purpose                    |
| -------------- | ------------ | -------------------------- |
| AHB1ENR        | `0x40023830` | Enable GPIO clocks         |
| APB2ENR        | `0x40023844` | Enable SYSCFG clock        |
| GPIOA_MODER    | `0x40020000` | Configure PA5 as output    |
| GPIOA_ODR      | `0x40020014` | Toggle LED                 |
| GPIOC_MODER    | `0x40020800` | Configure PC13 as input    |
| SYSCFG_EXTICR4 | `0x40013814` | Map PC13 to EXTI13         |
| EXTI_IMR       | `0x40013C00` | Interrupt Mask Register    |
| EXTI_FTSR      | `0x40013C0C` | Falling Trigger Selection  |
| EXTI_PR        | `0x40013C14` | Pending Register           |
| NVIC_ISER1     | `0xE000E104` | Enable EXTI15_10 interrupt |

---

## Expected Output

* On reset, the LED is OFF (or in its initial state).
* Each press of the push button on **PC13** generates an external interrupt.
* The interrupt toggles the LED connected to **PA5**.
* The main loop remains empty, demonstrating that the application is **entirely interrupt-driven**.

---

## Key Concepts Covered

* Bare-metal register programming
* GPIO configuration
* RCC clock enabling
* SYSCFG EXTI pin mapping
* External Interrupt (EXTI)
* Falling-edge detection
* Interrupt Mask Register (IMR)
* Pending Register (PR)
* NVIC interrupt enabling
* Cortex-M4 automatic context stacking
* Interrupt Service Routine (ISR)
* LED toggling using GPIO Output Data Register (ODR)

---

## Project Structure

```
STM32F411_EXTI/
│
├── main.c          // Application source
├── startup.s       // Startup file & vector table
├── linker.ld       // Linker script
├── Makefile        // Build configuration
└── README.md       // Project documentation
```

---

