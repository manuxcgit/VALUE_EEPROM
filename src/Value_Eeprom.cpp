#include "Value_Eeprom.h"

bool c_VALUE::_toUpdate;
static c_VALUE *ListeValues[15];

void c_VALUE::FIRST_USE(){
    for (size_t i = 0; i < NBR_VALUES; i++)
    {
        ListeValues[i] = 0;
    }    
} 

void c_VALUE::Init(int pdefaultValue, int pMini, int pMaxi, byte pIndex, bool ptoFlash, char* pName){
    for (size_t i = 0; i < NBR_VALUES; i++)
    {
        if (ListeValues[i] == 0){
            ListeValues[i] = this;
            break;
        }
    }
    index = pIndex;
    if (ptoFlash){
        value = EEPROM.read(index) + (255 * EEPROM.read(index + 1)) ;//+ 1 ;
        //pourquoi +1 ??? je ne sais pas, mais sinon manque à value ...
        if (value == 0xff00){
            update(pdefaultValue);
        }
    }
    else{
        update(pdefaultValue);
    }
    valueDefault = pdefaultValue;
    valueLast = value;
    mini = pMini;
    maxi = pMaxi;
    toFlash = ptoFlash;
    name = pName;
}

int c_VALUE::Get(){
    updated = false;
    return value;
}

///si true -> reste "non read" si updated par modif value
int c_VALUE::Get(bool pUpDated){
    updated &= pUpDated;
    return value; 
}

void c_VALUE::update(int pValue){
    value = pValue;
    if (value>valueLast){rising = 1;}
    else {rising = -1;}
    valueLast = pValue;
    updated = true;
    if (toFlash){
        EEPROM.write(index, (uint8_t)(value & 0xff));
        EEPROM.write(index + 1, (uint8_t)( value / 0xff));
        setToUpdate(true);
    }
}

void c_VALUE::Set(int pValue, bool pupDate, bool pcanInputValueOutOfRange){
    if (((pValue<mini) | (pValue>maxi)) & (!pcanInputValueOutOfRange)) {return;}
    if (!pupDate){
        value = pValue;
        return;
    }
    if (pValue != valueLast){
        update(pValue);
    }
    else {rising = 0;}
}

bool c_VALUE::commit(){
    if (_toUpdate){
        EEPROM.commit();
        _toUpdate = false;
        return true;
    }
    return false;
}

c_VALUE* c_VALUE::find(char* pName){
  for (size_t i = 0; i < NBR_VALUES; i++)
  {
    if (strcmp(ListeValues[i]->Name(), pName) == 0){
        return ListeValues[i];
    }
  }
  return nullptr;
}
