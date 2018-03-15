#ifndef __GDT_H
#define __GDT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class GlobalDescriptorTable{
public:
    class SegmentDiscriptor{
    private:
        uint16_t limitLow;
        uint16_t baseLow;
        uint8_t baseMid;
        uint8_t access;
        uint8_t flags;
        uint8_t baseHi;
        
    public:
        SegmentDiscriptor(uint32_t base, uint32_t limit, uint8_t type);
        uint32_t Base();
        uint32_t Limit();
        
    }__attribute__ ((packed));
    
private:
    SegmentDiscriptor nullSegmentSelector;
    SegmentDiscriptor unusedSegmentSelector;
    SegmentDiscriptor dataSegmentSelector;
    SegmentDiscriptor codeSegmentSelector;
    
public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();
    
    uint16_t DataSegmentSelector();
    uint16_t CodeSegmentSelector();

};
#endif
