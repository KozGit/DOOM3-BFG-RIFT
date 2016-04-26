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
idCVar vr_FBOEnabled( "vr_FBOEnabled", "1", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_RENDERER, "Use FBO rendering path." );
idCVar vr_FBOAAmode( "vr_FBOAAmode", "0", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_RENDERER, "Antialiasing mode. 0 = Disabled 1 = MSAA 2= FXAA\n" );
idCVar vr_enable( "vr_enable", "1", CVAR_INTEGER | CVAR_ARCHIVE | CVAR_GAME, "Enable VR mode. 0 = Disabled 1 = Enabled." );
idCVar vr_ipdOverride( "vr_ipdOverride", "0", CVAR_BOOL | CVAR_ARCHIVE, "Override Oculus IPD. 0 = use Oculus IPD\n 1 = use Manual IPD." );
idCVar vr_ipdManual( "vr_ipdManual", "64", CVAR_FLOAT | CVAR_ARCHIVE | CVAR_GAME, "User defined IPD value in MM" );

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
	
	hmdSession = nullptr;
	oculusSwapChain[0] = 0;
	oculusSwapChain[1] = 0;
	oculusFboId = 0;
	ocululsDepthTexID = 0;
	oculusMirrorFboId = 0;
	oculusMirrorTexture = 0;

	hmdEyeImage[0] = 0;
	hmdEyeImage[1] = 0;
	hmdCurrentRender[0] = 0;
	hmdCurrentRender[1] = 0;

	vrFrameNumber = 0;

			
}


/*
==============
iVr::HMDInit
==============
*/

void iVr::HMDInit( void ) 
{
	hasHMD = false;
	hasOculusRift = false;
	game->isVR = false;
	
	// Oculus HMD Initialization
	ovrResult result = ovr_Initialize( nullptr );
	
	if ( OVR_FAILURE( result ) )
	{
		common->Printf( "\nOculus Rift not detected.\n" );
		return;
	}


	common->Printf( "ovr_Initialize was successful.\n" );
	result  = ovr_Create( &hmdSession, & luid );
	
	if ( OVR_FAILURE( result ) )
	{
		common->Printf( "\nFailed to initialize Oculus Rift.\n" );
		ovr_Shutdown();
		return;
	}

	hmdDesc = ovr_GetHmdDesc( hmdSession );
	hasOculusRift = true;
	hasHMD = true;

	//ovrSizei resoultion = hmdDesc.Resolution;

	common->Printf( "\n\nOculus Rift HMD Initialized\n" );
	//ovr_RecenterPose( hmdSession ); // lets start looking forward.
	ovr_RecenterTrackingOrigin( hmdSession );
	hmdWidth = hmdDesc.Resolution.w;
	hmdHeight = hmdDesc.Resolution.h;
	vr->hmdHz = hmdDesc.DisplayRefreshRate + 1;
	com_engineHz.SetInteger( vr->hmdHz );
	common->Printf( "Hmd: %s .\n", hmdDesc.ProductName );
	common->Printf( "Hmd HZ %d, width %d, height %d\n", vr->hmdHz, hmdWidth, hmdHeight );
		
	ovr_GetAudioDeviceOutGuid( &oculusGuid );
	ovr_GetAudioDeviceOutGuidStr( oculusGuidStr );

	
}

/*
==============
iVr::HMDShutdown
==============
*/

void iVr::HMDShutdown( void )
{
	ovr_DestroyTextureSwapChain( hmdSession, oculusSwapChain[0] );
	ovr_DestroyTextureSwapChain( hmdSession, oculusSwapChain[1] );
	
	ovr_Destroy( hmdSession );
	ovr_Shutdown();
	return;
}


/*
==============
iVr::HMDInitializeDistortion
==============
*/

void iVr::HMDInitializeDistortion()
{

	
	
	if ( !vr->hmdSession || !vr->hasOculusRift || !vr_enable.GetBool() )
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
		hmdEye[eye].eyeRenderDesc = ovr_GetRenderDesc( vr->hmdSession, (ovrEyeType)eye, hmdEye[eye].eyeFov );
					
		common->Printf( "EYE %d viewoffset viewadjust x %f y %f z %f\n", eye, 
			hmdEye[eye].eyeRenderDesc.HmdToEyeOffset.x,
			hmdEye[eye].eyeRenderDesc.HmdToEyeOffset.y,
			hmdEye[eye].eyeRenderDesc.HmdToEyeOffset.z );
		

		ovrSizei rendertarget;
		ovrRecti viewport = { 0, 0, 0, 0 };

		rendertarget = ovr_GetFovTextureSize( vr->hmdSession, (ovrEyeType)eye, vr->hmdEye[eye].eyeFov, vr_pixelDensity.GetFloat() ); // make sure both eyes render to the same size target
				
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
	
	common->Printf( "Creating oculus texture set width = %d height = %d.\n", hmdEye[0].renderTarget.w, hmdEye[0].renderTarget.h );
		
	ovrTextureSwapChainDesc desc = {};
	desc.Type = ovrTexture_2D;
	desc.ArraySize = 1;
	desc.Width = hmdEye[0].renderTarget.w;
	desc.Height = hmdEye[0].renderTarget.h;
	desc.MipLevels = 1;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.SampleCount = 1;
	desc.StaticImage = ovrFalse;


	// create the swap texture sets 
	if (	ovr_CreateTextureSwapChainGL( hmdSession, &desc, &oculusSwapChain[0] ) != ovrSuccess ||
			ovr_CreateTextureSwapChainGL( hmdSession, &desc, &oculusSwapChain[1] ) != ovrSuccess )
	{
		common->Warning( "iVr::HMDInitializeDistortion unable to create OVR swap texture set.\n VR mode is DISABLED.\n" );
		game->isVR = false;

	}

	unsigned int texId = 0;
	int length = 0;
	for ( int j = 0; j < 2; j++ )
	{
		ovr_GetTextureSwapChainLength( hmdSession, oculusSwapChain[j], &length );
		for ( int i = 0; i < length; ++i )
		{
			ovr_GetTextureSwapChainBufferGL( hmdSession, oculusSwapChain[j], 0, &texId );
			//oculusSwapChainTexId[j] = texId;

			qglBindTexture( GL_TEXTURE_2D, texId );
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
		
	int ww = glConfig.nativeScreenWidth / 2;
	int wh = glConfig.nativeScreenHeight / 2;

	ovrMirrorTextureDesc mirrorDesc;
	memset( &mirrorDesc, 0, sizeof( mirrorDesc ) );
	mirrorDesc.Width = ww;
	mirrorDesc.Height = wh;
	mirrorDesc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
	mirrorW = ww;
	mirrorH = wh;

	common->Printf( "Creating oculus mirror texture %d x %d\n", ww, wh );
	ovr_CreateMirrorTextureGL( hmdSession, &mirrorDesc, &oculusMirrorTexture );
	ovr_GetMirrorTextureBufferGL( hmdSession, oculusMirrorTexture, &mirrorTexId );
	qglGenFramebuffers( 1, &oculusMirrorFboId );
	qglBindFramebuffer( GL_READ_FRAMEBUFFER, oculusMirrorFboId );
	qglFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexId, 0 );
	qglFramebufferRenderbuffer( GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0 );
	qglBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
	
	wglSwapIntervalEXT( 0 );
	
	oculusLayer.Header.Type = ovrLayerType_EyeFov;
	oculusLayer.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;
	oculusLayer.ColorTexture[0] = oculusSwapChain[0];
	oculusLayer.ColorTexture[1] = oculusSwapChain[1];
	oculusLayer.Fov[0] = hmdEye[0].eyeRenderDesc.Fov;
	oculusLayer.Fov[1] = hmdEye[1].eyeRenderDesc.Fov;
	oculusLayer.Viewport[0].Pos.x = 0;
	oculusLayer.Viewport[0].Pos.y = 0;
	oculusLayer.Viewport[0].Size.h = hmdEye[0].renderTarget.h;
	oculusLayer.Viewport[0].Size.w = hmdEye[0].renderTarget.w;
		
	oculusLayer.Viewport[1] = oculusLayer.Viewport[0];
		
	Framebuffer::BindDefault();
	wglSwapIntervalEXT( 0 );// make sure vsync is off.
	r_swapInterval.SetModified();
	
	GL_CheckErrors();

	// call this once now so the oculus layer has valid values to start with
	// when rendering the intro bink and splash screen.
	FrameStart(); 
	
	/*hmdTrackingState.CameraPose.Orientation.w = 1;
	hmdTrackingState.CameraPose.Orientation.x = 0;
	hmdTrackingState.CameraPose.Orientation.y = 0;
	hmdTrackingState.CameraPose.Orientation.z = 0;
	hmdTrackingState.HeadPose.ThePose.Orientation = hmdTrackingState.CameraPose.Orientation;
	hmdTrackingState.CameraPose.Position.x = 0.0f;
	hmdTrackingState.CameraPose.Position.y = 0.0f;
	hmdTrackingState.CameraPose.Position.z = 0.0f;
	hmdTrackingState.HeadPose.ThePose.Position = hmdTrackingState.CameraPose.Position;*/

	hmdTrackingState.HeadPose.ThePose.Orientation.w = 1;
	hmdTrackingState.HeadPose.ThePose.Orientation.x = 0;
	hmdTrackingState.HeadPose.ThePose.Orientation.y = 0;
	hmdTrackingState.HeadPose.ThePose.Orientation.z = 0;
	
	hmdTrackingState.HeadPose.ThePose.Position.x = 0.0f;
	hmdTrackingState.HeadPose.ThePose.Position.y = 0.0f;
	hmdTrackingState.HeadPose.ThePose.Position.z = 0.0f;
		
} 
		
/*
==============`
iVr::HMDGetOrientation
==============
*/

void iVr::HMDGetOrientation(float &roll, float &pitch, float &yaw, idVec3 &hmdPosition) 
{
	
	if ( game->IsPDAOpen() ) return; // hack to let timewarp translate the view when the PDA is up.

	
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
		
	if ( hasOculusRift && hmdSession ) 
	{
        
		if ( vr_trackingPredictionAuto.GetBool() ) 
		{
			hmdFrameTime = ovr_GetPredictedDisplayTime( hmdSession, 0 );// vrFrameNumber );
			time = hmdFrameTime;// .DisplayMidpointSeconds;
		} 
		else 
		{
			time = ovr_GetTimeInSeconds() + ( vr_trackingPredictionUserDefined.GetFloat() / 1000 );
		}
			
		hmdTrackingState = ovr_GetTrackingState( hmdSession, time,false );
				
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
	hmdPosition.y = -translationPose.Position.x * 39.37007874 ; // coordinates converted from oclus handedness to doom handedness.
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
void iVr::FrameStart( void ) 
{
	
	static idVec3 pos = vec3_zero;
	static float roll = 0;
	static float pitch = 0;
	static float yaw = 0;
	
	vr->HMDGetOrientation( roll, pitch, yaw, pos );
	
}

void iVr::HMDResetTrackingOrigin() {
	if ( hasOculusRift && vr->hmdSession ) {
		//ovr_RecenterPose( vr->hmdSession);
		ovr_RecenterTrackingOrigin( vr->hmdSession );
	}
}


//=========================================

//Console command to reset the oculus tracking origin
void vr_reset_origin_f( const idCmdArgs &args ) {
	vr->HMDResetTrackingOrigin();
}
static idCommandLink vr_reset_origin( "vr_reset_origin", vr_reset_origin_f, "Resets the Oculus tracking origin" );