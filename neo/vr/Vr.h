/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

//#include "precompiled.h"
#pragma hdrstop

#include "..\dependencies\LibOVR\Include\OVR_CAPI.h"
#include "..\dependencies\LibOVR\Include\OVR_CAPI_GL.h"
#include "vr_hmd.h"
#include "..\renderer\Framebuffer.h"
#include "..\dependencies\LibOVR\Include\OVR_CAPI_Audio.h"



#ifndef __VR_H__
#define __VR_H__

typedef enum 
{
	VR_AA_NONE,
	VR_AA_MSAA,
	VR_AA_FXAA,
	NUM_VR_AA
} vr_aa_t;

class iVr
{
public:
	
	iVr();
	
	void				HMDInit( void );
	void				HMDShutdown( void );
	void				HMDInitializeDistortion( void );
	void				HMDGetOrientation( float &roll, float &pitch, float &yaw, idVec3 &hmdPosition );
	void				HMDRender( idImage *leftCurrent, idImage *rightCurrent );
	void				HMDResetTrackingOrigin( void );
			
	void				FrameStart( void );

	uint				vrFrameNumber;
	
	float				angles[3];
	
	int					hmdWidth;
	int					hmdHeight;
	int					hmdHz;
	
	int					useFBO;
	int					primaryFBOWidth;
	int					primaryFBOHeight;
	
	int					VR_AAmode;
	
	int					VR_USE_HYDRA;
	
	bool				hasHMD;
	bool				hasOculusRift;
	
	ovrSession			hmdSession;
	ovrGraphicsLuid		luid;

	ovrHmdDesc			hmdDesc;

	float				hmdFovX;
	float				hmdFovY;
	hmdEye_t			hmdEye[2];
		
	float				oculusIPD;
	float				oculusHeight;
	float				manualIPD;
	float				manualHeight;

	idImage*			hmdEyeImage[2];
	idImage*			hmdCurrentRender[2];
	
	idImage*			primaryFBOimage;
	idImage*			resolveFBOimage;
	idImage*			fullscreenFBOimage;

	ovrTextureSwapChain oculusSwapChain[2];
	GLuint				oculusSwapChainTexId[2];
	
	GLuint				oculusFboId;
	GLuint				ocululsDepthTexID;
	
	ovrMirrorTexture	oculusMirrorTexture;
	GLuint				mirrorTexId;
	
	GLuint				oculusMirrorFboId;
	int					mirrorW;
	int					mirrorH;

	ovrLayerEyeFov		oculusLayer;
		
	ovrTrackingState	hmdTrackingState;
	double				hmdFrameTime;
	bool				hmdPositionTracked;

	GUID oculusGuid;
	WCHAR oculusGuidStr[OVR_AUDIO_MAX_DEVICE_STR_SIZE];


		
	//---------------------------
private:
	
};

#endif

extern idCVar	vr_ipdOverride;
extern idCVar	vr_ipdManual;

extern idCVar	vr_manualHeight;

extern idCVar	vr_guiScale; 
extern idCVar	vr_guiSeparation;

extern idCVar	vr_enable;

extern idCVar	vr_trackingPredictionAuto;
extern idCVar	vr_trackingPredictionUserDefined;

extern iVr* vr;

