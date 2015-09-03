
//#include "precompiled.h"
#pragma hdrstop

#include "d3xp\Game_local.h"
#include "vr.h"
#include "renderer\tr_local.h"
#include "sys\win32\win_local.h"
#include "..\dependencies\LibOVR\Include\OVR_CAPI_GL.h"

void GLimp_SwapBuffers();
void GL_BlockingSwapBuffers();

extern PFNWGLSWAPINTERVALEXTPROC				wglSwapIntervalEXT;

idCVar vr_showOvrSubmitFrameTime( "vr_showOvrSubmitFrameTime", "0", CVAR_BOOL, "Show time elapsed during OvrSubmitFrame call\n" );

/*
====================
iVr::HMDRender

Draw the pre rendered eye textures to the back buffer.
Apply FXAA if enabled.
Apply HMD distortion correction.

eye textures: idImage leftCurrent, rightCurrent
====================
*/

void iVr::HMDRender ( idImage *leftCurrent, idImage *rightCurrent ) 
{
	
	extern ovrFrameTiming	hmdFrameTime;
	static ovrLayerHeader	*layers = &oculusLayer.Header;
	static ovrPosef			eyeRenderPose[2];
	static ovrVector3f		viewOffset[2] = { hmdEye[0].eyeRenderDesc.HmdToEyeViewOffset, hmdEye[1].eyeRenderDesc.HmdToEyeViewOffset };
	static ovrViewScaleDesc viewScaleDesc;

	static GLint windowW = glConfig.nativeScreenWidth / 4;
	static GLint windowH = glConfig.nativeScreenHeight / 4;
	static GLint texW = oculusMirrorTexture->OGL.Header.TextureSize.w;
	static GLint texH = oculusMirrorTexture->OGL.Header.TextureSize.h;
	static GLint windowTexW = windowW + texW;
	static GLint windowTexH = windowH + texH;

	// increment the oculus texture swap chain indexes
	for ( int i = 0; i < 2; i++ )
	{
		oculusTextureSet[i]->CurrentIndex = (oculusTextureSet[i]->CurrentIndex + 1) % oculusTextureSet[i]->TextureCount;
	}
			
	renderProgManager.BindShader_PostProcess(); // pass thru shader
		
	//render stereoEye images into oculus textures.
	//left eye
	ovrGLTexture* tex = (ovrGLTexture*)&oculusTextureSet[0]->Textures[oculusTextureSet[0]->CurrentIndex];
	qglBindFramebuffer( GL_FRAMEBUFFER, oculusFboId );
	qglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->OGL.TexId, 0 );
	qglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ocululsDepthTexID, 0 );
	GL_ViewportAndScissor( 0, 0, tex->OGL.Header.TextureSize.w, tex->OGL.Header.TextureSize.h );
	qglClearColor(0, 0, 0, 0 );
	qglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	
	GL_CheckErrors();
		
	// draw the left eye texture.				
	GL_SelectTexture( 0 );
	leftCurrent->Bind();
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	RB_DrawElementsWithCounters( &backEnd.unitSquareSurface ); // draw it

	
	//--------------------
	//right eye
	//--------------------
	tex = (ovrGLTexture*)&oculusTextureSet[1]->Textures[oculusTextureSet[1]->CurrentIndex];
	qglBindFramebuffer( GL_FRAMEBUFFER, oculusFboId );
	qglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->OGL.TexId, 0 );
	qglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ocululsDepthTexID, 0 );
	GL_ViewportAndScissor( 0, 0, tex->OGL.Header.TextureSize.w, tex->OGL.Header.TextureSize.h );
	qglClearColor( 0, 0, 0, 0 );
	qglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// draw the right eye texture
	
	GL_SelectTexture( 0 );
	rightCurrent->Bind();
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	RB_DrawElementsWithCounters( &backEnd.unitSquareSurface ); // draw it
	 
	renderProgManager.Unbind();
		
	// Submit frame/layer to oculus compositor
	qglBindFramebuffer( GL_FRAMEBUFFER, oculusFboId );
	qglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
	qglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 );

	ovr_CalcEyePoses( vr->hmdTrackingState.HeadPose.ThePose, viewOffset, eyeRenderPose ); 
		
	viewScaleDesc.HmdSpaceToWorldScaleInMeters = 0.0254f; // inches to meters
	viewScaleDesc.HmdToEyeViewOffset[0] = hmdEye[0].eyeRenderDesc.HmdToEyeViewOffset;
	viewScaleDesc.HmdToEyeViewOffset[1] = hmdEye[1].eyeRenderDesc.HmdToEyeViewOffset;
	
	oculusLayer.RenderPose[0] = eyeRenderPose[0];
	oculusLayer.RenderPose[1] = eyeRenderPose[1];
	
	static int beforeSubmit = Sys_Milliseconds();
	ovrResult result = ovr_SubmitFrame( hmd, 0, &viewScaleDesc, &layers, 1 );
	if ( result != ovrSuccess )
	{
		common->Warning( "Vr_GL.cpp HMDRender : Failed to submit oculus layer.\n" );
	}
	static int afterSubmit = Sys_Milliseconds();
	
	if ( vr_showOvrSubmitFrameTime.GetBool() )
	{
		common->Printf( "ovrHmd_SubmitFrame start %d end %d time %d\n", beforeSubmit, afterSubmit, afterSubmit - beforeSubmit );
	}
	if ( vr->useFBO )
	{
		// Blit mirror texture to back buffer
		qglBindFramebuffer( GL_READ_FRAMEBUFFER, oculusMirrorFboId );
		qglBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		qglBlitFramebuffer( 0, texH, texW, 0, windowW, windowH, windowTexW, windowTexH, GL_COLOR_BUFFER_BIT, GL_NEAREST );
		qglBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
	}
	
	// koz hack
	// for some reason, vsync will not disable unless wglSwapIntervalEXT( 0 )
	// is called at least once after ovr_SubmitFrame is called.
	// (at least on the two Nvidia cards I have to test with.)
	// this only seems to be the case when rendering to FBOs instead
	// of the default framebuffer.
	// if anyone has any ideas why this is, please tell!
	
	static int swapset = 0;
	if ( swapset == 0 )
	{
		swapset = 1;
		wglSwapIntervalEXT( 0 );
	}
	
}

