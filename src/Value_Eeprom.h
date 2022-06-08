#pragma once

//juste pour upload MAJ

#include <Arduino.h>
#include "FlashAsEEPROM.h"

#define NBR_VALUES 15
class c_VALUE{
    int value, valueLast, valueDefault;
    int maxi, mini;
    byte index, rising; // rising = 1 si value > valuelast, 0 pour equal et -1 pour <
    bool toFlash, updated; //updated = true si MAJ value depuis dernère lecture
    static bool _toUpdate;
    char* name;

    void update(int pValue);

public:
    inline static c_VALUE *ListeValues[NBR_VALUES];

    void FIRST_USE(); //initilise les values à 0 
    void Init(int pDefaultValue, int pMini, int pMaxi, byte pIndex, bool ptoFlash, char* pName);
    void Set(int pValue, bool pupDate, bool pcanInputValueOutOfRange);
    void SetToDefault(){update(valueDefault);}
    void Reset(){value = valueLast;}
    int Get();
    int Get(bool pUpDatedRead);
    /// rising = 1 si value > valuelast, 0 pour equal et -1 pour <
    byte Rising(){return rising;}
    int Max(){return maxi;}
    int Min(){return mini;}
    char* Name(){return name;}
    /// true si value modified et non read
    bool Updated(){return updated;}
    /// pour forcer MAJ affichage
    void Update(){updated = true;}
    static void setToUpdate(bool b){ _toUpdate = b;}
    static bool commit();
    static c_VALUE* find(char* pName);
};