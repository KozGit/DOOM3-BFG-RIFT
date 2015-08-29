#pragma hdrstop

//#include"precompiled.h"

#undef strncmp
#undef vsnprintf		
#undef _vsnprintf		

#include "vr.h"
#include "d3xp\Game_local.h"
#include "sys\win32\win_local.h"
#include "d3xp\physics\Clip.h"
#include "..\dependencies\LibOVR\Include\OVR_CAPI_GL.h"
#include "..\renderer\Framebuffer.h"

#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / idMath::PI))


// *** Oculus HMD Variables

idCVar vr_trackingPredictionAuto( "vr_useAutoTrackingPrediction", "1", CVAR_BOOL | CVAR_ARCHIVE | CVAR_GAME, "Use SDK tracking prediction.\n 1 = Auto, 0 = User defined." );
idCVar vr_trackingPredictionUserDefined( "vr_trackingPredictionUserDefined", "50", CVAR_FLOAT | CVAR_ARCHIVE | CVAR_GAME, "User defined tracking prediction in ms." );
idCVar vr_pixelDensity( "vr_pixelDensity", "1.25", CVAR_FLOAT | CVAR_ARCHIVE | CVAR_GAME, "" );
idCVar vr_FBOEnabled( "vr_FBOEnabled", "0", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_RENDERER, "Use FBO rendering path." );
idCVar vr_FBOAAmode( "vr_FBOAAmode", "1", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_RENDERER, "Antialiasing mode. 0 = Disabled 1 = MSAA 2= FXAA\n" );
idCVar vr_enable( "vr_enable", "1", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_GAME, "Enable VR mode. 0 = Disabled 1 = Enabled." );
idCVar vr_useOculusProfile( "vr_useOculusProfile", "1", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_GAME, "Use Oculus Profile values. 0 = use user defined profile, 1 = use Oculus profile." );
idCVar vr_manualIPD( "vr_manualIPD", "64", CVAR_FLOAT | CVAR_ARCHIVE | CVAR_GAME, "User defined IPD value in MM" );
idCVar vr_manualHeight( "vr_manualHeight", "70", CVAR_FLOAT | CVAR_ARCHIVE | CVAR_GAME, "User defined player height in inches" );

idCVar vr_guiScale( "vr_guiScale", "1", CVAR_FLOAT | CVAR_RENDERER | CVAR_ARCHIVE, "scale reduction factor for full screen menu/pda scale in VR", 0.0001f, 1.0f ); //koz allow scaling of full screen guis/pda
idCVar vr_guiSeparation( "vr_guiSeparation", ".01", CVAR_FLOAT | CVAR_ARCHIVE, " Screen separation value for fullscreen guis." );


// Koz end
//===================================================================

int fboWidth;
int fboHeight;

iVr vrLocal;
iVr* vr = &vrLocal;

/*
====================
R_MakeFBOImage
====================
*/
static void R_MakeFBOImage( idImage* image )
{
	idImageOpts	opts;
	opts.width = fboWidth;
	opts.height = fboHeight;
	opts.numLevels = 1;
	opts.format = FMT_RGBA8;
	image->AllocImage( opts, TF_LINEAR, TR_CLAMP );
}

/*
==============
iVr::iVr()
==============
*/
iVr::iVr()
{
	hasHMD = false;
	hasOculusRift = false;
	useFBO = 0;
	VR_USE_HYDRA = 0;
	
	oculusIPD = 64.0f;
	oculusHeight = 72.0f;
		
	hmdPositionTracked = false;
	
	VR_AAmode = 0;
			
	hmdWidth =  0;
	hmdHeight = 0;
					
	primaryFBOWidth = 0;
	primaryFBOHeight = 0;
	hmdHz = 75; // koz fixme what doesnt support 60 hz?
	
	hmd = nullptr;
	oculusTextureSet[0] = 0;
	oculusTextureSet[1] = 0;
	oculusFboId = 0;
	ocululsDepthTexID = 0;
	oculusMirrorFboId = 0;
	oculusMirrorTexture = 0;

	hmdEyeImage[0] = 0;
	hmdEyeImage[1] = 0;
	hmdCurrentRender[0] = 0;
	hmdCurrentRender[1] = 0;
			
}


/*
==============
iVr::HMDInit
==============
*/

void iVr::HMDInit( void ) 
{
	
	// Oculus HMD Initialization
	
	if ( ovr_Initialize( nullptr ) == ovrSuccess )
	{
		common->Printf( "ovr_Initialize was successful.\n" );
		ovrResult result  = ovr_Create( &hmd, & luid );
		if ( result == ovrSuccess )
		
		{
			
			hmdDesc = ovr_GetHmdDesc( hmd );

			// configure the tracking
			common->Printf( "ovrHmd_Create was successful.\n" );
			if ( ovr_ConfigureTracking( hmd, ovrTrackingCap_Orientation
												| ovrTrackingCap_MagYawCorrection
												| ovrTrackingCap_Position, ovrTrackingCap_Orientation ) == ovrSuccess )
			{
				
				common->Printf( "ovrHmd_ConfigureTracking was successful.\n" );
				hasOculusRift = true;
				hasHMD = true;
				common->Printf( "\n\nOculus Rift HMD Initialized\n" );
								
				
				ovr_SetEnabledCaps( hmd, hmdDesc.DefaultHmdCaps );
				ovr_RecenterPose( hmd ); // lets start looking forward.
							
								
				hmdWidth = hmdDesc.Resolution.w;
				hmdHeight = hmdDesc.Resolution.h;
				
				vr->hmdHz = hmdDesc.DisplayRefreshRate;
				
				com_engineHz.SetInteger( vr->hmdHz );
				
				common->Printf( "Hmd: %s .\n", hmdDesc.ProductName );
				common->Printf( "Hmd HZ %d, width %d, height %d\n", vr->hmdHz, hmdWidth, hmdHeight );
				return;
			}

		}
	}
	
	common->Printf( "\nOculus Rift not detected.\n" );
	hasHMD = false;
	hasOculusRift = false;
	game->isVR = false;

}



/*
==============
iVr::HMDInitializeDistortion
==============
*/

void iVr::HMDInitializeDistortion()
{

	if ( !vr->hmd || !vr->hasOculusRift || !vr_enable.GetBool() )
	{
		game->isVR = false;
		return;
	}

	game->isVR = true;
	common->Printf( "VR Mode ENABLED.\n" );
	
	useFBO = vr_FBOEnabled.GetInteger() && glConfig.framebufferObjectAvailable;

	if ( vr_FBOEnabled.GetInteger() && !glConfig.framebufferObjectAvailable )
	{
		common->Printf( "Framebuffer requested but framebufferObject not available.\n Reverting to default GL framebuffer.\n" );
		useFBO = false;
	}
	bool fboCreated = false;
		
	for ( int eye = 0; eye < 2; eye++ )
	{
		
		hmdEye[eye].eyeFov = vr->hmdDesc.DefaultEyeFov[eye];
		hmdEye[eye].eyeRenderDesc = ovr_GetRenderDesc( vr->hmd, (ovrEyeType)eye, hmdEye[eye].eyeFov );
		
		//hmdEye[eye].projection.x.scale = 2.0f / (hmdEye[eye].eyeFov.LeftTan + hmdEye[eye].eyeFov.RightTan);
		//hmdEye[eye].projection.x.offset = (hmdEye[eye].eyeFov.LeftTan - hmdEye[eye].eyeFov.RightTan) * hmdEye[eye].projection.x.scale * 0.5f;
		//hmdEye[eye].projection.y.scale = 2.0f / (hmdEye[eye].eyeFov.UpTan + hmdEye[eye].eyeFov.DownTan);
		//hmdEye[eye].projection.y.offset = (hmdEye[eye].eyeFov.UpTan - hmdEye[eye].eyeFov.DownTan) * hmdEye[eye].projection.y.scale * 0.5f;

		hmdEye[eye].viewOffset = (idVec3)(-hmdEye[eye].eyeRenderDesc.HmdToEyeViewOffset.x,
			hmdEye[eye].eyeRenderDesc.HmdToEyeViewOffset.y,
			hmdEye[eye].eyeRenderDesc.HmdToEyeViewOffset.z);

		common->Printf( "EYE %d px.scale %f, px.offset %f, py.scale %f, py.offset %f\n", eye, hmdEye[eye].projection.x.scale, hmdEye[eye].projection.x.offset, hmdEye[eye].projection.y.scale, hmdEye[eye].projection.y.offset );
		common->Printf( "EYE %d viewoffset viewadjust x %f y %f z %f\n", eye, hmdEye[eye].viewOffset.x, hmdEye[eye].viewOffset.y, hmdEye[eye].viewOffset.z );

		ovrSizei rendertarget;
		ovrRecti viewport = { 0, 0, 0, 0 };

		rendertarget = ovr_GetFovTextureSize( vr->hmd, (ovrEyeType)eye, vr->hmdEye[eye].eyeFov, vr_pixelDensity.GetFloat() ); // make sure both eyes render to the same size target
		hmdEye[eye].renderTarget.h = rendertarget.h; 
		hmdEye[eye].renderTarget.w = rendertarget.w;
		common->Printf( "Eye %d Rendertaget Width x Height = %d x %d\n", eye, rendertarget.w, rendertarget.h );

		if ( useFBO && !fboCreated )
		{
			common->Printf( "Generating FBOs.\n" );
			common->Printf( "Requested pixel density = %f \n", vr_pixelDensity.GetFloat() );
			primaryFBOWidth = rendertarget.w;
			primaryFBOHeight = rendertarget.h;
			fboWidth = rendertarget.w;
			fboHeight = rendertarget.h;


			if ( !fboCreated )
			{ // create the FBOs if needed.

				VR_AAmode = vr_FBOAAmode.GetInteger();
				if ( r_multiSamples.GetInteger() >= 1 ) vr->VR_AAmode = VR_AA_MSAA;
				common->Printf( "Rmultisamples = %d, vr_FBOAAmode = %d \n", r_multiSamples.GetInteger(), vr_FBOAAmode.GetInteger() );
				if ( VR_AAmode == VR_AA_MSAA && r_multiSamples.GetInteger() == 0 ) VR_AAmode = VR_AA_NONE;

				if ( VR_AAmode == VR_AA_FXAA )
				{// enable FXAA

					VR_AAmode = VR_AA_NONE;
				}

				if ( VR_AAmode == VR_AA_MSAA )
				{// enable MSAA
					common->Printf( "Creating %d x %d MSAA framebuffer\n", rendertarget.w, rendertarget.h );
					globalFramebuffers.primaryFBO = new Framebuffer( "_primaryFBO", rendertarget.w, rendertarget.h, true ); // koz
					common->Printf( "Adding Depth/Stencil attachments to MSAA framebuffer\n" );
					globalFramebuffers.primaryFBO->AddDepthStencilBuffer( GL_DEPTH24_STENCIL8 );
					common->Printf( "Adding color attachment to MSAA framebuffer\n" );
					globalFramebuffers.primaryFBO->AddColorBuffer( GL_RGBA8, 0 );

					int status = globalFramebuffers.primaryFBO->Check();
					globalFramebuffers.primaryFBO->Error( status );

					resolveFBOimage = globalImages->ImageFromFunction( "_resolveFBOimage", R_MakeFBOImage );

					common->Printf( "Creating resolve framebuffer\n" );
					globalFramebuffers.resolveFBO = new Framebuffer( "_resolveFBO", rendertarget.w, rendertarget.h, false ); // koz

					common->Printf( "Adding Depth/Stencil attachments to MSAA framebuffer\n" );
					globalFramebuffers.resolveFBO->AddDepthStencilBuffer( GL_DEPTH24_STENCIL8 );

					common->Printf( "Adding color attachment to framebuffer\n" );
					globalFramebuffers.resolveFBO->AttachImage2D( GL_TEXTURE_2D, resolveFBOimage, 0 );
					resolveFBOimage->Bind();

					status = globalFramebuffers.resolveFBO->Check();
					globalFramebuffers.resolveFBO->Error( status );


					fboWidth = rendertarget.w * 2;
					fullscreenFBOimage = globalImages->ImageFromFunction( "_fullscreenFBOimage", R_MakeFBOImage );

					common->Printf( "Creating fullscreen framebuffer\n" );
					globalFramebuffers.fullscreenFBO = new Framebuffer( "_fullscreenFBO", rendertarget.w * 2, rendertarget.h, false ); // koz

					common->Printf( "Adding color attachment to framebuffer\n" );
					fullscreenFBOimage->Bind();
					globalFramebuffers.fullscreenFBO->Bind();
					globalFramebuffers.fullscreenFBO->AttachImage2D( GL_TEXTURE_2D, fullscreenFBOimage, 0 );


					status = globalFramebuffers.fullscreenFBO->Check();
					globalFramebuffers.fullscreenFBO->Error( status );

					fboWidth = rendertarget.w;

					if ( status == GL_FRAMEBUFFER_COMPLETE )
					{
						useFBO = 1;
						fboCreated = true;
					}
					else
					{
						useFBO = 0;
						fboCreated = false;
					}
										
				}

				if ( !fboCreated /*!VR_FBO.valid*/ )
				{ // either AA disabled or AA buffer creation failed. Try creating unaliased FBOs.

					primaryFBOimage = globalImages->ImageFromFunction( "_primaryFBOimage", R_MakeFBOImage );
					common->Printf( "Creating framebuffer\n" );
					globalFramebuffers.primaryFBO = new Framebuffer( "_primaryFBO", rendertarget.w, rendertarget.h, false ); // koz

					common->Printf( "Adding Depth/Stencil attachments to framebuffer\n" );
					globalFramebuffers.primaryFBO->AddDepthStencilBuffer( GL_DEPTH24_STENCIL8 );

					common->Printf( "Adding color attachment to framebuffer\n" );
					globalFramebuffers.primaryFBO->AttachImage2D( GL_TEXTURE_2D, primaryFBOimage, 0 );
					primaryFBOimage->Bind();

					int status = globalFramebuffers.primaryFBO->Check();
					globalFramebuffers.primaryFBO->Error( status );

					fboWidth = rendertarget.w * 2;
					fullscreenFBOimage = globalImages->ImageFromFunction( "_fullscreenFBOimage", R_MakeFBOImage );

					common->Printf( "Creating fullscreen framebuffer\n" );
					globalFramebuffers.fullscreenFBO = new Framebuffer( "_fullscreenFBO", rendertarget.w * 2, rendertarget.h, false ); // koz

					common->Printf( "Adding color attachment to framebuffer\n" );
					fullscreenFBOimage->Bind();
					globalFramebuffers.fullscreenFBO->Bind();
					globalFramebuffers.fullscreenFBO->AttachImage2D( GL_TEXTURE_2D, fullscreenFBOimage, 0 );


					status = globalFramebuffers.fullscreenFBO->Check();
					globalFramebuffers.fullscreenFBO->Error( status );

					if ( status == GL_FRAMEBUFFER_COMPLETE )
					{
						useFBO = 1;
						fboCreated = true;
					}
					else
					{
						useFBO = false;
						fboCreated = false;
					}
				}
			}
		}

		if ( !useFBO ) { // not using FBO's, will render to default framebuffer (screen) 

			rendertarget.w = renderSystem->GetNativeWidth() / 2;
			rendertarget.h = renderSystem->GetNativeHeight();
			hmdEye[eye].renderTarget = rendertarget;
		}

		viewport.Size.w = rendertarget.w;
		viewport.Size.h = rendertarget.h;
		
	}

	// calculate fov for engine
	float combinedTanHalfFovHorizontal = std::max( std::max( hmdEye[0].eyeFov.LeftTan, hmdEye[0].eyeFov.RightTan ), std::max( hmdEye[1].eyeFov.LeftTan, hmdEye[1].eyeFov.RightTan ) );
	float combinedTanHalfFovVertical = std::max( std::max( hmdEye[0].eyeFov.UpTan, hmdEye[0].eyeFov.DownTan ), std::max( hmdEye[1].eyeFov.UpTan, hmdEye[1].eyeFov.DownTan ) );
	float horizontalFullFovInRadians = 2.0f * atanf( combinedTanHalfFovHorizontal );

	hmdFovX = RAD2DEG( horizontalFullFovInRadians );
	hmdFovY = RAD2DEG( 2.0 * atanf( combinedTanHalfFovVertical ) );
	
	common->Warning( "Init Hmd FOV x,y = %f , %f. \n", hmdFovX, hmdFovY );
	common->Printf( "Creating oculus texture set width = %d height = %d.\n", hmdEye[0].renderTarget.w * 2, hmdEye[0].renderTarget.h );
	
	// create the swap texture sets 
	if ( (ovr_CreateSwapTextureSetGL( hmd, GL_SRGB8_ALPHA8, hmdEye[0].renderTarget.w, hmdEye[0].renderTarget.h,
		&oculusTextureSet[0] ) != ovrSuccess) || ovr_CreateSwapTextureSetGL( hmd, GL_SRGB8_ALPHA8, hmdEye[0].renderTarget.w, hmdEye[0].renderTarget.h,
		&oculusTextureSet[1] ) != ovrSuccess ) {
		common->Warning( "iVr::HMDInitializeDistortion unable to create OVR swap texture set.\n VR mode is DISABLED.\n" );
		game->isVR = false;

	}

	for ( int j = 0; j < 2; j++ )
	{
	
		for ( int i = 0; i < oculusTextureSet[j]->TextureCount; ++i )
		{
			ovrGLTexture* tex = (ovrGLTexture*)&oculusTextureSet[j]->Textures[i];
			qglBindTexture( GL_TEXTURE_2D, tex->OGL.TexId );

			qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		}
	}
	
	qglGenFramebuffers( 1, &oculusFboId );
	qglGenTextures( 1, &ocululsDepthTexID );
	qglBindTexture( GL_TEXTURE_2D, ocululsDepthTexID );
	qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	qglTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, hmdEye[0].renderTarget.w, hmdEye[0].renderTarget.h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL );
	
	float as = vr->hmdWidth / vr->hmdHeight;
	int ww = 800;
	int wh = int( 800 / as );
	
	ovr_CreateMirrorTextureGL( hmd, GL_RGBA, ww, wh, (ovrTexture**) &oculusMirrorTexture );
	qglGenFramebuffers( 1, &oculusMirrorFboId );
	qglBindFramebuffer( GL_READ_FRAMEBUFFER, oculusMirrorFboId );
	qglFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, oculusMirrorTexture->OGL.TexId, 0 );
	qglFramebufferRenderbuffer( GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0 );
	qglBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );

	oculusLayer.Header.Type = ovrLayerType_EyeFov;
	oculusLayer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;
	oculusLayer.ColorTexture[0] = oculusTextureSet[0];
	oculusLayer.ColorTexture[1] = oculusTextureSet[1];
	oculusLayer.Fov[0] = hmdEye[0].eyeRenderDesc.Fov;
	oculusLayer.Fov[1] = hmdEye[1].eyeRenderDesc.Fov;
	oculusLayer.Viewport[0].Pos.x = 0;
	oculusLayer.Viewport[0].Pos.y = 0;
	oculusLayer.Viewport[0].Size.h = hmdEye[0].renderTarget.h;
	oculusLayer.Viewport[0].Size.w = hmdEye[0].renderTarget.w;
		
	oculusLayer.Viewport[1] = oculusLayer.Viewport[0];
		
	wglSwapIntervalEXT( 0 );// make sure vsync is off.
	r_swapInterval.SetModified();
	
	GL_CheckErrors();
} 
		
/*
==============`
iVr::HMDGetOrientation
==============
*/

void iVr::HMDGetOrientation(float &roll, float &pitch, float &yaw, idVec3 &hmdPosition) 
{
	
	if ( !hasOculusRift || !hasHMD ) 
	{
		roll = 0;
		pitch =0;
		yaw = 0;
		hmdPosition = idVec3 ( 0, 0, 0 ) ;
		return;
	}
	static double time = 0.0;
	static ovrPosef translationPose;
	static ovrPosef	orientationPose; 
	static ovrPosef lastTrackedPose = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;
	static bool currentlyTracked;
		
	if ( hasOculusRift && hmd ) 
	{
        
		if ( vr_trackingPredictionAuto.GetBool() ) 
		{
			hmdFrameTime = ovr_GetFrameTiming( hmd, idLib::frameNumber );
			time = hmdFrameTime.DisplayMidpointSeconds;
		} 
		else 
		{
			time = ovr_GetTimeInSeconds() + ( vr_trackingPredictionUserDefined.GetFloat() / 1000 );
		}
			
		hmdTrackingState = ovr_GetTrackingState( hmd, time );
				
		if (hmdTrackingState.StatusFlags & ( ovrStatus_OrientationTracked ) )
		{
							
				orientationPose = hmdTrackingState.HeadPose.ThePose; 
				idQuat poseQuat;
				idAngles poseAngles;

				poseQuat.w = orientationPose.Orientation.w;
				poseQuat.x = orientationPose.Orientation.z;
				poseQuat.y = orientationPose.Orientation.x;
				poseQuat.z = -orientationPose.Orientation.y;

				poseAngles = poseQuat.ToAngles();
				
				roll = poseAngles.roll;
				pitch = poseAngles.pitch;
				yaw = poseAngles.yaw;

		}
		
	}
	else 
	{
			roll  = angles[ROLL];
			pitch = angles[PITCH];
			yaw   = angles[YAW];
	}
		
	// now read the HMD position if equiped

	currentlyTracked = hmdTrackingState.StatusFlags & ( ovrStatus_PositionTracked ) ? 1 : 0;
	//common->Printf("currently tracked = %d\n", currentlyTracked);

	if ( currentlyTracked ) 
	{
			translationPose = hmdTrackingState.HeadPose.ThePose; 
			lastTrackedPose = translationPose;
	} 
	else
	{	
			translationPose = lastTrackedPose;
	}
				
	hmdPosition.x = -translationPose.Position.z * 39.37007874 ; // koz convert position (in meters) to inch (1 id unit = 1 inch). ( m * 39.37007874 = inch )   
	hmdPosition.y = -translationPose.Position.x * 39.37007874 ; // coordinates converted from ouclus handedness to doom handedness.
	hmdPosition.z = translationPose.Position.y * 39.37007874 ;
			
	if ( hmdDesc.AvailableTrackingCaps && (ovrTrackingCap_Position) ) 
	{
		if ( currentlyTracked && !hmdPositionTracked ) 
		{
			common->Warning( "Oculus Positional tracking established.\n" );
			hmdPositionTracked = true;
		} 
		else if ( !currentlyTracked && hmdPositionTracked ) 
		{
			common->Warning( "Oculus Positional tracking LOST.\n" );
			hmdPositionTracked = false;
		}
	}
}

/*
==============
iVr::FrameStart
==============
*/
void iVr::FrameStart( int index ) 
{
	// koz fixme
			
	static idVec3 pos = vec3_zero;
	static float roll = 0;
	static float pitch = 0;
	static float yaw = 0;

	vr->HMDGetOrientation( roll, pitch, yaw, pos );
	
}

void iVr::HMDResetTrackingOrigin() {
	if ( hasOculusRift && vr->hmd ) {
		ovr_RecenterPose( vr->hmd );
	}
}


//=========================================

//Console command to reset the oculus tracking origin
void vr_reset_origin_f( const idCmdArgs &args ) {
	vr->HMDResetTrackingOrigin();
}
static idCommandLink vr_reset_origin( "vr_reset_origin", vr_reset_origin_f, "Resets the Oculus tracking origin" );