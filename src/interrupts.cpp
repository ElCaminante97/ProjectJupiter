#include <interrupts.h>

void terminal_writestring(const char* str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
uint16_t InterruptManager::hardwareInterruptOffset;

InterruptHandler::InterruptHandler(InterruptManager* interruptManager, uint8_t InterruptNumber){
    this->InterruptNumber = InterruptNumber;
    this->Interruptmanager = interruptManager;
    Interruptmanager->handlers[InterruptNumber] = this;
}

InterruptHandler::~InterruptHandler(){
    if(Interruptmanager->handlers[InterruptNumber] == this){
        Interruptmanager->handlers[InterruptNumber] = 0;
    }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp){
    return esp;
}

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

 void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interrupt, uint16_t codeSegmentSelectorOffset, void (*handler)(), uint8_t descriptorPrivilageLevel, uint8_t descriptorType){
     interruptDescriptorTable[interrupt].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
     interruptDescriptorTable[interrupt].handlerAddressHighBits = ((uint32_t) handler >> 16) & 0xFFFF;
      interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
      
      const uint8_t IDT_DEC_PRESENT = 0x80;
       interruptDescriptorTable[interrupt].access = IDT_DEC_PRESENT | ((descriptorPrivilageLevel & 3) << 5) | descriptorType;
    interruptDescriptorTable[interrupt].reserved = 0;
}
 
 InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt)
 : programmableInterruptControllerMasterCommandPort(0x20),
 programmableInterruptControllerMasterDataPort(0x21),
 programmableInterruptControllerSlaveCommandPort(0xA0),
 programmableInterruptControllerSlaveDataPort(0xA1)
 {
     InterruptManager::hardwareInterruptOffset = hardwareInterruptOffset;
     uint32_t CodeSegment = gdt->CodeSegmentSelector();
     const uint8_t IDT_INTERRUPT_GATE = 0xE;
     for(uint8_t i = 255; i > 0; i--){
         setInterruptDescriptorTableEntry(i, CodeSegment, &interruptIgnore, 0, IDT_INTERRUPT_GATE);
         handlers[i] = 0;
    }
    setInterruptDescriptorTableEntry(0, CodeSegment, &interruptIgnore, 0, IDT_INTERRUPT_GATE);
    handlers[0] = 0;
    
    setInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);
    
   setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
     setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);
    
    programmableInterruptControllerMasterCommandPort.Write(0x11);
    programmableInterruptControllerSlaveCommandPort.Write(0x11);
    
    programmableInterruptControllerMasterDataPort.Write(hardwareInterruptOffset);
    programmableInterruptControllerSlaveDataPort.Write(hardwareInterruptOffset+8);
    
    programmableInterruptControllerMasterDataPort.Write(0x4);
    programmableInterruptControllerSlaveDataPort.Write(0x2);
    
    programmableInterruptControllerMasterDataPort.Write(0x1);
    programmableInterruptControllerSlaveDataPort.Write(0x1);
    
    programmableInterruptControllerMasterDataPort.Write(0x0);
    programmableInterruptControllerSlaveDataPort.Write(0x0);
    
    InterruptDescriptorPointer idtPointer;
    idtPointer.size = 256*sizeof(GateDescriptor) -1;
    idtPointer.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idtPointer));
    
}


InterruptManager::~InterruptManager(){
    Deactivate();
}

uint16_t InterruptManager::HardwareInterruptOffset(){
    return InterruptManager::hardwareInterruptOffset;
}

void InterruptManager::Activate(){
    if(ActiveInterruptManager != 0)
        ActiveInterruptManager->Deactivate();
    
    ActiveInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate(){
    if(ActiveInterruptManager == this){
        ActiveInterruptManager = 0;
        asm("cli");
    }
}
    
uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt, uint32_t esp){
   if(ActiveInterruptManager != 0){
       return ActiveInterruptManager->DoHandleInterrupt(interrupt, esp);
    }
    return esp;
}


 uint32_t InterruptManager::DoHandleInterrupt(uint8_t interrupt, uint32_t esp){
     if(handlers[interrupt] != 0){
        //esp = handlers[interrupt]->HandleInterrupt(esp);
    }else if(interrupt != hardwareInterruptOffset){
        char* foo = "Unhandled Interrupt 0x00";
        char* hex = "0123456789ABCDEF";
        
        foo[22] = hex[(interrupt >> 4) & 0xF];
        foo[23] = hex[interrupt & 0xF];
        
        terminal_writestring(foo + '\n');
    }
    
    /*if(hardwareInterruptOffset <= interrupt && interrupt < hardwareInterruptOffset+ 16){
        programmableInterruptControllerMasterCommandPort.Write(0x20);
        if(hardwareInterruptOffset+8 <= interrupt){
            programmableInterruptControllerSlaveCommandPort.Write(0x20);
        }
    }*/
    
    return esp;
}
