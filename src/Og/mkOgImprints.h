#ifndef MKOGIMPRINTS_H_INCLUDED
#define MKOGIMPRINTS_H_INCLUDED

#include <Og/mkOgConfig.h>
#include <Og/mkOgForward.h>
#include <Og/mkOgTypes.h>

#include <Utils/mkUtils.h>
#include <Utils/Reflect/mkBaseImprint.h>
#include <Object/mkLibrary.h>
#include <Utils/Reflect/mkImprint.h>
#include <Utils/Reflect/mkInjector.h>
#include <Utils/Reflect/mkMethod.h>
#include <Utils/Memory/mkMemory.h>
#include <Utils/Memory/mkMultiPool.h>
#include <Utils/Memory/mkPoolMake.h>
#include <Object/Util/mkMake.h>

namespace mk
{
    
#ifdef IMPRINT_DEF_FILE
    

    
	void imprintmkOg(std::vector<Type*>& types, std::vector<Type*>& prototypes)
	{
		// Just in case
		UNUSED(types); UNUSED(prototypes);
		
        // Base Types
    
        // Enums
    
        // Sequences
    
        // Stores
    
   
        // OgreModule
        OgreModule::cls()->setupImprint(make_unique<Imprint>(OgreModule::cls(), "OgreModule", OBJECT));
        OgreModule::cls()->imprint()->initObject(false);
        
        
        types.push_back(OgreModule::cls());
	}
#endif // IMPRINT_DEF_FILE
}

#endif // MKOGIMPRINTS_H_INCLUDED