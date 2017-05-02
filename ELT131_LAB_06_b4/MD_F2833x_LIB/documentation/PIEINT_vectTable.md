### Table 111. PIE MUXed Peripheral Interrupt Vector Table (SPRUFB0D.pdf:124) ###

<div style="font-size: 9pt;" markdown=1>


| Group   | INTx.8      | INTx.7      | INTx.6      | INTx.5      | INTx.4      | INTx.3      | INTx.2      | INTx.1      |
|:--------|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|
| INT1.y  | WAKEINT     | TINT0       | ADCINT      | XINT2       | XINT1       | Reserved    | SEQ2INT     | SEQ1INT     |
|         | (LPM/WD)    | (TIMER 0)   | (ADC)       | Ext-Int_2   | Ext-Int_1   | -           | (ADC)       | (ADC)       |
|         | 0xD4E       | 0xD4C       | 0xD4A       | 0xD48       | 0xD46       | 0xD44       | 0xD42       | 0xD40       |
| IRQ ID  |       39    |       38    |       37    |       36    |       35    |       34    |       33    |       32    |
| INT2.y  | Reserved    | Reserved    | EPWM6_TZINT | EPWM5_TZINT | EPWM4_TZINT | EPWM3_TZINT | EPWM2_TZINT | EPWM1_TZINT |
|         | -           | -           | (ePWM6)     | (ePWM5)     | (ePWM4)     | (ePWM3)     | (ePWM2)     | (ePWM1)     |
|         | 0xD5E       | 0xD5C       | 0xD5A       | 0xD58       | 0xD56       | 0xD54       | 0xD52       | 0xD50       |
| IRQ ID  |       47    |       46    |       45    |       44    |       43    |       42    |       41    |       40    |
| INT3.y  | Reserved    | Reserved    | EPWM6_INT   | EPWM5_INT   | EPWM4_INT   | EPWM3_INT   | EPWM2_INT   | EPWM1_INT   |
|         | -           | -           | (ePWM6)     | (ePWM5)     | (ePWM4)     | (ePWM3)     | (ePWM2)     | (ePWM1)     |
|         | 0xD6E       | 0xD6C       | 0xD6A       | 0xD68       | 0xD66       | 0xD64       | 0xD62       | 0xD60       |
| IRQ ID  |       55    |       54    |       53    |       52    |       51    |       50    |       49    |       48    |
| INT4.y  | Reserved    | Reserved    | ECAP6_INT   | ECAP5_INT   | ECAP4_INT   | ECAP3_INT   | ECAP2_INT   | ECAP1_INT   |
|         | -           | -           | (eCAP6)     | (eCAP5)     | (eCAP4)     | (eCAP3)     | (eCAP2)     | (eCAP1)     |
|         | 0xD7E       | 0xD7C       | 0xD7A       | 0xD78       | 0xD76       | 0xD74       | 0xD72       | 0xD70       |
| IRQ ID  |       63    |       62    |       61    |       60    |       59    |       58    |       57    |       56    |
| INT5.y  | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | EQEP2_INT   | EQEP1_INT   |
|         | -           | -           | -           | -           | -           | -           | (eQEP2)     | (eQEP1)     |
|         | 0xD8E       | 0xD8C       | 0xD8A       | 0xD88       | 0xD86       | 0xD84       | 0xD82       | 0xD80       |
| IRQ ID  |       71    |       70    |       69    |       68    |       67    |       66    |       65    |       64    |
| INT6.y  | Reserved    | Reserved    | MXINTA      | MRINTA      | MXINTB      | MRINTB      | SPITXINTA   | SPIRXINTA   |
|         | (McBSP-A)   | (McBSP-A)   | (McBSP-B)   | (McBSP-B)   | (SPI-A)     | (SPI-A)     |             |             |
|         | 0xD9E       | 0xD9C       | 0xD9A       | 0xD98       | 0xD96       | 0xD94       | 0xD92       | 0xD90       |
| IRQ ID  |       79    |       78    |       77    |       76    |       75    |       74    |       73    |       72    |
| INT7.y  | Reserved    | Reserved    | DINTCH6     | DINTCH5     | DINTCH4     | DINTCH3     | DINTCH2     | DINTCH1     |
|         | (DMA6)      | (DMA5)      | (DMA4)      | (DMA3)      | (DMA2)      | (DMA1)      |             |             |
|         | 0xDAE       | 0xDAC       | 0xDAA       | 0xDA8       | 0xDA6       | 0xDA4       | 0xDA2       | 0xDA0       |
| IRQ ID  |       87    |       86    |       85    |       84    |       83    |       82    |       81    |       80    |
| INT8.y  | Reserved    | Reserved    | SCITXINTC   | SCIRXINTC   | Reserved    | Reserved    | I2CINT2A    | I2CINT1A    |
|         | -           | -           | (SCI-C)     | (SCI-C)     | -           | -           | (I2C-A)     | (I2C-A)     |
|         | 0xDBE       | 0xDBC       | 0xDBA       | 0xDB8       | 0xDB6       | 0xDB4       | 0xDB2       | 0xDB0       |
| IRQ ID  |       95    |       94    |       93    |       92    |       91    |       90    |       89    |       88    |
| INT9.y  | ECAN1INTB   | ECAN0INTB   | ECAN1INTA   | ECAN0INTA   | SCITXINTB   | SCIRXINTB   | SCITXINTA   | SCIRXINTA   |
|         | (CAN-B)     | (CAN-B)     | (CAN-A)     | (CAN-A)     | (SCI-B)     | (SCI-B)     | (SCI-A)     | (SCI-A)     |
|         | 0xDCE       | 0xDCC       | 0xDCA       | 0xDC8       | 0xDC6       | 0xDC4       | 0xDC2       | 0xDC0       |
| IRQ ID  |             |             |             |             |             |             |             |       96    |
| INT10.y | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    |
|         | -           | -           | -           | -           | -           | -           | -           | -           |
|         | 0xDDE       | 0xDDC       | 0xDDA       | 0xDD8       | 0xDD6       | 0xDD4       | 0xDD2       | 0xDD0       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT11.y | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    |             |
|         | -           | -           | -           | -           | -           | -           | -           | -           |
|         | -           | -           | -           | -           | -           | -           | -           | -           |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT12.y | LUF         | LVF         | Reserved    | XINT7       | XINT6       | XINT5       | XINT4       | XINT3       |
|         | (FPU)       | (FPU)       | -           | Ext-Int_7   | Ext-Int_6   | Ext-Int_5   | Ext-Int_4   | Ext-Int_3   |
|         | 0xDFE       | 0xDFC       | 0xDFA       | 0xDF8       | 0xDF6       | 0xDF4       | 0xDF2       | 0xDF0       |
| IRQ ID  |             |             |             |             |             |             |             |             |


</div>

<!-- 
| Group   | INTx.8      | INTx.7      | INTx.6      | INTx.5      | INTx.4      | INTx.3      | INTx.2      | INTx.1      |
|:--------|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|:-----------:|
| INT1.y  | WAKEINT     | TINT0       | ADCINT      | XINT2       | XINT1       | Reserved    | SEQ2INT     | SEQ1INT     |
|         | (LPM/WD)    | (TIMER 0)   | (ADC)       | Ext-Int_2   | Ext-Int_1   | -           | (ADC)       | (ADC)       |
|         | 0xD4E       | 0xD4C       | 0xD4A       | 0xD48       | 0xD46       | 0xD44       | 0xD42       | 0xD40       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT2.y  | Reserved    | Reserved    | EPWM6_TZINT | EPWM5_TZINT | EPWM4_TZINT | EPWM3_TZINT | EPWM2_TZINT | EPWM1_TZINT |
|         | -           | -           | (ePWM6)     | (ePWM5)     | (ePWM4)     | (ePWM3)     | (ePWM2)     | (ePWM1)     |
|         | 0xD5E       | 0xD5C       | 0xD5A       | 0xD58       | 0xD56       | 0xD54       | 0xD52       | 0xD50       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT3.y  | Reserved    | Reserved    | EPWM6_INT   | EPWM5_INT   | EPWM4_INT   | EPWM3_INT   | EPWM2_INT   | EPWM1_INT   |
|         | -           | -           | (ePWM6)     | (ePWM5)     | (ePWM4)     | (ePWM3)     | (ePWM2)     | (ePWM1)     |
|         | 0xD6E       | 0xD6C       | 0xD6A       | 0xD68       | 0xD66       | 0xD64       | 0xD62       | 0xD60       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT4.y  | Reserved    | Reserved    | ECAP6_INT   | ECAP5_INT   | ECAP4_INT   | ECAP3_INT   | ECAP2_INT   | ECAP1_INT   |
|         | -           | -           | (eCAP6)     | (eCAP5)     | (eCAP4)     | (eCAP3)     | (eCAP2)     | (eCAP1)     |
|         | 0xD7E       | 0xD7C       | 0xD7A       | 0xD78       | 0xD76       | 0xD74       | 0xD72       | 0xD70       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT5.y  | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | EQEP2_INT   | EQEP1_INT   |
|         | -           | -           | -           | -           | -           | -           | (eQEP2)     | (eQEP1)     |
|         | 0xD8E       | 0xD8C       | 0xD8A       | 0xD88       | 0xD86       | 0xD84       | 0xD82       | 0xD80       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT6.y  | Reserved    | Reserved    | MXINTA      | MRINTA      | MXINTB      | MRINTB      | SPITXINTA   | SPIRXINTA   |
|         | (McBSP-A)   | (McBSP-A)   | (McBSP-B)   | (McBSP-B)   | (SPI-A)     | (SPI-A)     |             |             |
|         | 0xD9E       | 0xD9C       | 0xD9A       | 0xD98       | 0xD96       | 0xD94       | 0xD92       | 0xD90       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT7.y  | Reserved    | Reserved    | DINTCH6     | DINTCH5     | DINTCH4     | DINTCH3     | DINTCH2     | DINTCH1     |
|         | (DMA6)      | (DMA5)      | (DMA4)      | (DMA3)      | (DMA2)      | (DMA1)      |             |             |
|         | 0xDAE       | 0xDAC       | 0xDAA       | 0xDA8       | 0xDA6       | 0xDA4       | 0xDA2       | 0xDA0       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT8.y  | Reserved    | Reserved    | SCITXINTC   | SCIRXINTC   | Reserved    | Reserved    | I2CINT2A    | I2CINT1A    |
|         | -           | -           | (SCI-C)     | (SCI-C)     | -           | -           | (I2C-A)     | (I2C-A)     |
|         | 0xDBE       | 0xDBC       | 0xDBA       | 0xDB8       | 0xDB6       | 0xDB4       | 0xDB2       | 0xDB0       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT9.y  | ECAN1INTB   | ECAN0INTB   | ECAN1INTA   | ECAN0INTA   | SCITXINTB   | SCIRXINTB   | SCITXINTA   | SCIRXINTA   |
|         | (CAN-B)     | (CAN-B)     | (CAN-A)     | (CAN-A)     | (SCI-B)     | (SCI-B)     | (SCI-A)     | (SCI-A)     |
|         | 0xDCE       | 0xDCC       | 0xDCA       | 0xDC8       | 0xDC6       | 0xDC4       | 0xDC2       | 0xDC0       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT10.y | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    |
|         | -           | -           | -           | -           | -           | -           | -           | -           |
|         | 0xDDE       | 0xDDC       | 0xDDA       | 0xDD8       | 0xDD6       | 0xDD4       | 0xDD2       | 0xDD0       |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT11.y | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    | Reserved    |             |
|         | -           | -           | -           | -           | -           | -           | -           | -           |
|         | -           | -           | -           | -           | -           | -           | -           | -           |
| IRQ ID  |             |             |             |             |             |             |             |             |
| INT12.y | LUF         | LVF         | Reserved    | XINT7       | XINT6       | XINT5       | XINT4       | XINT3       |
|         | (FPU)       | (FPU)       | -           | Ext-Int_7   | Ext-Int_6   | Ext-Int_5   | Ext-Int_4   | Ext-Int_3   |
|         | 0xDFE       | 0xDFC       | 0xDFA       | 0xDF8       | 0xDF6       | 0xDF4       | 0xDF2       | 0xDF0       |
| IRQ ID  |             |             |             |             |             |             |             |             |
 -->