//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#pragma once

#include "BsPrerequisites.h"
#include "BsTextSprite.h"
#include "BsGUIContent.h"

namespace BansheeEngine
{
	/**
	 * @brief	Contains various helper methods used by GUI.
	 */
	class BS_EXPORT GUIHelper
	{
	public:
		/**
		 * @brief	Calculates optimal content size by returning the nearest valid size to the provided value.
		 *
		 * @param	contentSize		Wanted content size. This will be limited by minimal constraints of the style and layout options.
		 * @param	style			Style to use for determining size constraints.
		 * @param	layoutOptions	Layout options to use for determining size constraints.
		 */
		static Vector2I calcOptimalContentsSize(const Vector2I& contentSize, const GUIElementStyle& style, const GUILayoutOptions& layoutOptions);

		/**
		 * @brief	Calculates optimal content size for the provided content using the provided style and layout options for constraints.
		 *
		 * @param	content			Content to calculate size for.
		 * @param	style			Style to use for determining size constraints.
		 * @param	layoutOptions	Layout options to use for determining size constraints.
		 */
		static Vector2I calcOptimalContentsSize(const GUIContent& content, const GUIElementStyle& style, const GUILayoutOptions& layoutOptions);

		/**
		 * @brief	Calculates optimal content size for the provided text using the provided style and layout options for constraints.
		 *
		 * @param	text			Text to calculate size for.
		 * @param	style			Style to use for determining size constraints.
		 * @param	layoutOptions	Layout options to use for determining size constraints.
		 */
		static Vector2I calcOptimalContentsSize(const WString& text, const GUIElementStyle& style, const GUILayoutOptions& layoutOptions);
	};
}