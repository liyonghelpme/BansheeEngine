//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#pragma once

#include "BsCorePrerequisites.h"

namespace BansheeEngine
{
	/**
	 * @brief	Factory class that you may specialize in order to start up
	 *			a specific render system.
	 */
	class RenderSystemFactory
	{
	public:
		/**
		 * @brief	Creates and starts up the render system managed by this factory. 
		 */
		virtual void create() = 0;

		/**
		 * @brief	Returns the name of the render system this factory creates.
		 */
		virtual const String& name() const = 0;
	};
}