#include "camera.h"
#include "common.h"
#include "common_matrix.h"

void Init_CAMERA4DV1( CAMERA4DV1_PTR camera, int attr, POINT4D_PTR world_pos, VECTOR4D_PTR cam_dir, POINT4D_PTR cam_target,
					 float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height )
{
	camera->attr = attr;
	VECTOR4D_COPY( &camera->pos, world_pos );
	VECTOR4D_COPY( &camera->dir, cam_dir );

	VECTOR4D_INITXYZ( &camera->u, 1, 0, 0 );
	VECTOR4D_INITXYZ( &camera->v, 0, 1, 0 );
	VECTOR4D_INITXYZ( &camera->n, 0, 0, 1 );
	if( cam_target )
	{
		VECTOR4D_COPY( &camera->target, cam_target );
	}
	else
	{
		VECTOR4D_ZERO( &camera->target );
	}

	camera->near_clip_z = near_clip_z;
	camera->far_clip_z = far_clip_z;

	camera->fov = fov;

	camera->viewport_width = viewport_width;
	camera->viewport_height = viewport_height;
	camera->aspect_ratio = viewport_width/viewport_height;
	camera->viewport_center_x = ( viewport_width - 1.0f )/2;
	camera->viewport_center_y = ( viewport_height - 1.0f )/2;


	camera->viewplane_width = 2;
	camera->viewplane_height = camera->viewplane_width / camera->aspect_ratio;

	float tan_half_fov = tan( DEG_TO_RAD( fov/2.0f ) );
	camera->view_dist = 0.5f * camera->viewplane_width / tan_half_fov ;

	MAT_IDENTITY_4X4( &camera->mcam );
	MAT_IDENTITY_4X4( &camera->mper );
	MAT_IDENTITY_4X4( &camera->mscr );

	if ( fov == 90.0f )
	{
		POINT3D pt_origin = { 0, 0, 0 };
		VECTOR3D vn;

		// right
		VECTOR3D_INITXYZ( &vn, 1, 0, -1 );
		PLANE3D_INIT( &camera->rt_clip_plane, &pt_origin, &vn, 1 );

		// left
		VECTOR3D_INITXYZ( &vn, -1, 0, -1 );
		PLANE3D_INIT( &camera->lt_clip_plane, &pt_origin, &vn, 1 );

		// top
		VECTOR3D_INITXYZ( &vn, 0, 1, -1 );
		PLANE3D_INIT( &camera->tp_clip_plane, &pt_origin, &vn, 1 );
		
		// bottom
		VECTOR3D_INITXYZ( &vn, 0, -1, -1 );
		PLANE3D_INIT( &camera->bt_clip_plane, &pt_origin, &vn, 1 );
	}
	else
	{
		POINT3D pt_origin = { 0, 0, 0 };
		VECTOR3D vn;

		// right
		VECTOR3D_INITXYZ( &vn, camera->view_dist, 0, -camera->viewport_width/2.0f );
		PLANE3D_INIT( &camera->rt_clip_plane, &pt_origin, &vn, 1 );

		// LEFT
		VECTOR3D_INITXYZ( &vn, -camera->view_dist, 0, -camera->viewport_width/2.0f );
		PLANE3D_INIT( &camera->lt_clip_plane, &pt_origin, &vn, 1 );

		// TOP
		VECTOR3D_INITXYZ( &vn, 0, camera->view_dist, -camera->viewport_height/2.0f );
		PLANE3D_INIT( &camera->tp_clip_plane, &pt_origin, &vn, 1 );

		// BOTTOM
		VECTOR3D_INITXYZ( &vn, 0, -camera->view_dist, -camera->viewport_height/2.0f );
		PLANE3D_INIT( &camera->bt_clip_plane, &pt_origin, &vn, 1 );
	}
	
}

void Build_CAM4DV1_Matrix_Euler( CAMERA4DV1_PTR cam, int cam_rot_seq )
{
	MATRIX4X4 mt_inv, mx_inv, my_inv, mz_inv, mrot, mtmp;

	MAT_INIT_4X4( &mt_inv, 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cam->pos.x, -cam->pos.y, -cam->pos.z, 1 );

	float theta_x = -cam->dir.x;
	float theta_y = -cam->dir.y;
	float theta_z = -cam->dir.z;

	// inv rotation matrix X
	float cos_theta = cos( DEG_TO_RAD( theta_x ) );
	float sin_theta = sin( DEG_TO_RAD( theta_x ) );

	MAT_INIT_4X4( &mx_inv, 
		1, 0, 0, 0, 
		0, cos_theta, sin_theta, 0,
		0, -sin_theta, cos_theta, 0, 
		0, 0, 0, 1 );

	// inv rotation matrix Y
	cos_theta = cos( DEG_TO_RAD( theta_y ) );
	sin_theta = sin( DEG_TO_RAD( theta_y ) );

	MAT_INIT_4X4( &my_inv, 
		cos_theta, 0, -sin_theta, 0,
		0, 1, 0, 0,
		sin_theta, 0, cos_theta, 0,
		0, 0, 0, 1 );

	// inv rotation matrix z
	cos_theta = cos( DEG_TO_RAD( theta_z ) );
	sin_theta = sin( DEG_TO_RAD( theta_z ) );

	MAT_INIT_4X4( &mz_inv, 
		cos_theta, sin_theta, 0, 0, 
		-sin_theta, cos_theta, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 );

	switch ( cam_rot_seq )
	{
	case CAM_ROT_SEQ_XYZ:
		{
			MAT_MUL_4X4( &mx_inv, &my_inv, &mtmp );
			MAT_MUL_4X4( &mtmp, &mz_inv, &mrot );
		}
		break;
	case CAM_ROT_SEQ_YXZ:
		{
			MAT_MUL_4X4( &my_inv, &mx_inv, &mtmp );
			MAT_MUL_4X4( &mtmp, &mz_inv, &mrot );
		}
		break;
	case CAM_ROT_SEQ_XZY:
		{
			MAT_MUL_4X4( &mx_inv, &mz_inv, &mtmp );
			MAT_MUL_4X4( &mtmp, &my_inv, &mrot );
		}
		break;
	case CAM_ROT_SEQ_YZX:
		{
			MAT_MUL_4X4( &my_inv, &mz_inv, &mtmp );
			MAT_MUL_4X4( &mtmp, &mx_inv, &mrot );
		}
		break;
	case CAM_ROT_SEQ_ZYX:
		{
			MAT_MUL_4X4( &mz_inv, &my_inv, &mtmp );
			MAT_MUL_4X4( &mtmp, &mx_inv, &mrot );
		}
		break;
	case CAM_ROT_SEQ_ZXY:
		{
			MAT_MUL_4X4( &mz_inv, &mx_inv, &mtmp );
			MAT_MUL_4X4( &mtmp, &my_inv, &mrot );
		}
		break;
	default:
		break;
	}

	MAT_MUL_4X4( &mt_inv, &mrot, &cam->mcam );
}

void Build_CAM4DV1_Matrix_UVN( CAMERA4DV1_PTR cam, int uvn_mode )
{
	MATRIX4X4 mt, mt_uvn;

	MAT_INIT_4X4( &mt, 
		1, 0, 0, 0,
		0, 1, 0, 0, 
		0, 0, 1, 0,
		-cam->pos.x, -cam->pos.y, -cam->pos.z, 1 );

	if( uvn_mode == UVN_TARGET_MODE_SPHERICAL )
	{
		float phi = cam->dir.x;
		float theta = cam->dir.y;

		float sin_phi = sin( DEG_TO_RAD( phi ) );
		float cos_phi = cos( DEG_TO_RAD( phi ) );

		float sin_theta = sin( DEG_TO_RAD( theta ) );
		float cos_theta = cos( DEG_TO_RAD( theta ) );

		cam->target.y = 1 * sin_phi;
		cam->target.x = - 1 * cos_phi * sin_theta;
		cam->target.z = 1 * cos_phi * cos_theta;
	}

	VECTOR4D_SUB( &cam->target, &cam->pos, &cam->n );
	VECTOR4D_INITXYZ( &cam->v, 0, 1, 0 );
	VECTOR4D_CROSS( &cam->v, &cam->n, &cam->u );
	VECTOR4D_CROSS( &cam->n, &cam->u, &cam->v );

	VECTOR4D_NORMALIZE( &cam->u );
	VECTOR4D_NORMALIZE( &cam->v );
	VECTOR4D_NORMALIZE( &cam->n );

	MAT_INIT_4X4( &mt_uvn, 
		cam->u.x, cam->v.x, cam->n.x, 0, 
		cam->u.y, cam->v.y, cam->n.y, 0, 
		cam->u.z, cam->v.z, cam->n.z, 0, 
		0, 0, 0, 1);

	MAT_MUL_4X4( &mt, &mt_uvn, &cam->mcam );
}

void Build_CAM4DV1_To_Perspective_MATRIX4X4( CAMERA4DV1_PTR cam, MATRIX4X4_PTR mt );
{
	// MAT_INIT_4X4( &mt, 

}