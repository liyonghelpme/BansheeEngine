//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#pragma once

#include "BsGLPrerequisites.h"
#include "BsGLContext.h"
#include "BsGLPixelBuffer.h"
#include "BsPixelData.h"

namespace BansheeEngine 
{
	/**
	 * @brief	Describes OpenGL frame buffer surface.
	 */
    struct BS_RSGL_EXPORT GLSurfaceDesc
    {
    public:
		GLSurfaceDesc() 
			:zoffset(0), numSamples(0) 
		{ }

		GLPixelBufferPtr buffer;
		UINT32 zoffset;
		UINT32 numSamples;
    };

	/**
	 * @brief	Manages an OpenGL frame-buffer object. Frame buffer object
	 *			is used as a rendering destination in the render system pipeline,
	 *			and it may consist out of one or multiple color surfaces and an optional
	 *			depth/stencil surface.
	 */
    class BS_RSGL_EXPORT GLFrameBufferObject
    {
    public:
        GLFrameBufferObject();
        ~GLFrameBufferObject();

		/**
		 * @brief	Binds a color surface to the specific attachment point.
		 *
		 * @param	attachment	Attachment point index in the range [0, BS_MAX_MULTIPLE_RENDER_TARGETS).
		 * @param	target		Description of the color surface to attach.
		 *
		 * @note	Multisample counts of all surfaces must match.
		 *			0th attachment must be bound in order for the object to be usable, rest are optional.
		 */
        void bindSurface(UINT32 attachment, const GLSurfaceDesc& target);

		/**
		 * @brief	Unbinds the attachment at the specified attachment index.
		 *
		 * @param	attachment	Attachment point index in the range [0, BS_MAX_MULTIPLE_RENDER_TARGETS).
		 */
        void unbindSurface(UINT32 attachment);

		/**
		 * @brief	Binds a depth/stencil buffer.
		 *
		 * @note	Multisample counts of depth/stencil and color surfaces must match.
		 *			Binding a depth/stencil buffer is optional.
		 */
		void bindDepthStencil(GLPixelBufferPtr depthStencilBuffer);

		/**
		 * @brief	Unbinds a depth stencil buffer.
		 */
		void unbindDepthStencil();
        
		/**
		 * @brief	Binds the frame buffer object to the OpenGL pipeline, making it used
		 *			for any further rendering operations.
		 */
        void bind();

		/**
		 * @brief	Returns internal OpenGL frame buffer id.
		 */
		GLuint getGLFBOID() const { return mFB; }

    private:
		/**
		 * @brief	Rebuilds internal frame buffer object. Should be called whenever surfaces change.
		 */
        void rebuild();

	private:
        GLuint mFB;

        GLSurfaceDesc mColor[BS_MAX_MULTIPLE_RENDER_TARGETS];
		GLPixelBufferPtr mDepthStencilBuffer;
    };
}