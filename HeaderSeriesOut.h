#pragma once

#include "main.h"

struct Header{
    uint8_t tileSet;
    uint8_t BG0_Properties;
    uint8_t BG1_Properties;
    uint8_t BG2_Properties;
    uint8_t BG3_Properties;
    uint32_t BG0_DataPointer;
    uint32_t BG1_DataPointer;
    uint32_t BG2_DataPointer;
    uint32_t clipDataPointer;
    uint32_t BG3_DataPointer;
    uint8_t BG3_Scrolling;
    uint8_t transparency;
    uint32_t defaultSpriteDataPointer;
    uint8_t defaultSpriteSet;
    uint8_t firstSpriteSetEvent;
    uint32_t firstSpriteDataPointer;
    uint8_t firstSpriteSet;
    uint8_t secondSpriteEvent;
    uint32_t secondSpriteDataPointer;
    uint8_t secondSpriteSet;
    uint8_t mapXCoordinate;
    uint8_t mapYCoordinate;
    uint8_t effect;
    uint8_t effectYPosition;
    uint16_t musicTrackNumber;
}Head;
