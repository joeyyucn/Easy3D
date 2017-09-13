#include <stdio.h>
#include <string.h>
#include <math.h>
#include "file_plg.h"
#define PLG_MAX_LINE_LENGTH		256

#define PLG_COLOR_MODE_MASK		0x8000
#define PLG_SHADE_MODE_MASK		0x6000
#define PLG_2SIDE_MASK			0x1000
#define PLG_COLOR_MASK			0x0fff

#define PLG_COLOR_MODE_RGB_FLAG	0x8000
#define PLG_COLOR_MODE_INDEXED_FLAG	0x0000

#define PLG_2SIDE_FLAG			0x1000
#define PLG_1SIDE_FLAG			0X0000

#define PLG_SHADE_MODE_PURE_FLAG	0x0000
#define PLG_SHADE_MODE_FLAT_FLAG	0x2000
#define PLG_SHADE_MODE_GOURAUD_FLAG	0x4000
#define PLG_SHADE_MODE_PHONG_FLAG	0x6000

inline bool isSpace(char character)
{
	return character == ' ';
}

static char* plg_get_line(char* buffer, int maxlength, FILE* fp)
{
	int length, index = 0;
	while(true)
	{
		if(!fgets(buffer, maxlength, fp))
		{
			return NULL;
		}

		for(length = strlen(buffer); isSpace(buffer[index]);index++);

		// blank line
		if(index == (length-1))
			continue;

		// comment line
		if(buffer[index] == '#')
			continue;

		return &buffer[index];
	}
}

static void compute_object4dv1_radius(object4dv1_ptr obj)
{
	if(obj->num_vertices == 0)
	{
		obj->max_radius = obj->avg_radius = 0;
	}
	else
	{
		float total_distance = 0;
		float radius, max_radius = 0;
		point4d_ptr point = NULL;
		for(int i = 0; i < obj->num_vertices; i++)
		{
			point = &obj->vlist_local[i];

			radius = sqrt(point->x * point->x + point->y * point->y + point->z * point->z);
			if(radius > max_radius)
				max_radius = radius;
			total_distance += radius;
		}

		obj->max_radius = max_radius;
		obj->avg_radius = total_distance/(float)obj->num_vertices;
	}
}

int plg_load_object4dv1(object4dv1_ptr obj, char* filename, vector4d_ptr pos, vector4d_ptr rotate, vector4d_ptr scale)
{
	FILE* pFile = NULL;
	char buffer[PLG_MAX_LINE_LENGTH];
	char* token_string = NULL;
	int poly_num_vertices, poly_surface_desc;
	char temp_string[10];

	if(!(pFile = fopen(filename, "r")))
	{
		printf("[PLG] failed open file %s.\n", filename);
		return 0;
	}

	memset(buffer, 0, PLG_MAX_LINE_LENGTH);

	// world pos
	obj->world_pos.x = pos->x;
	obj->world_pos.y = pos->y;
	obj->world_pos.z = pos->z;

	obj->state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;

	// name & num_vertices & num_polys
	token_string = plg_get_line(buffer, PLG_MAX_LINE_LENGTH, pFile);
	if(!token_string)
	{
		printf("[PLG] can not find any descriptor from file %s\n", filename);
		return NULL;
	}
	sscanf(token_string, "%s %d %d", &obj->name, &obj->num_vertices, &obj->num_polys);

	// vertices 
	for(int vertex = 0; vertex	 < obj->num_vertices; vertex++)
	{
		if(!(token_string = plg_get_line(buffer, PLG_MAX_LINE_LENGTH, pFile)))
		{
			printf("[PLG] invalid vertex list with file %s", filename);
			return 0;
		}

		sscanf(token_string, "%f %f %f", &obj->vlist_local[vertex].x, &obj->vlist_local[vertex].y, &obj->vlist_local[vertex].z);
		obj->vlist_local[vertex].w = 1;

		obj->vlist_local[vertex].x *= scale->x;
		obj->vlist_local[vertex].y *= scale->y;
		obj->vlist_local[vertex].z *= scale->z;
	}

	// average radius and max radius
	compute_object4dv1_radius(obj);

	// polygons
	poly4dv1_ptr pPoly = NULL;
	for(int polygon = 0; polygon < obj->num_polys; polygon++)
	{
		pPoly = &obj->plist[polygon];
		if(!(token_string = plg_get_line(buffer, PLG_MAX_LINE_LENGTH, pFile)))
		{
			printf("[PLG] invalid polygon list with file %s", filename);
			return 0;
		}
		
		sscanf(token_string, "%s %d %d %d %d", &temp_string, &poly_num_vertices,
			&obj->plist[polygon].vindex[0], &obj->plist[polygon].vindex[1], &obj->plist[polygon].vindex[2]);
		obj->plist[polygon].vlist = obj->vlist_local;
		
		if(temp_string[0]=='0' && temp_string[1]=='x')
			sscanf(temp_string, "%x", &poly_surface_desc);
		else
			sscanf(temp_string, "%d", &poly_surface_desc);

		// side mode
		if(poly_surface_desc & PLG_2SIDE_MASK)
		{
			pPoly->attr &= POLY4DV1_ATTR_2SIDE;
		}

		// color mode
		if(poly_surface_desc & PLG_COLOR_MASK)
		{
			pPoly->attr &= POLY4DV1_ATTR_RGB16;
			pPoly->color = 0;
			int red = (poly_surface_desc & 0x0f00) >> 8;
			int green = (poly_surface_desc & 0x00f0) >> 4;
			int blue = (poly_surface_desc & 0x000f);

			pPoly->color |= red << 20;
			pPoly->color |= green << 12;
			pPoly->color |= blue << 4;
		}
		else
		{
			pPoly->attr |= POLY4DV1_ATTR_8BITCOLOR;
			pPoly->color = poly_surface_desc & 0x00ff;
		}

		//shade mode
		pPoly->attr = 0;
		int shade_mode = poly_surface_desc & PLG_SHADE_MODE_MASK;
		switch(shade_mode)
		{
		case PLG_SHADE_MODE_PURE_FLAG:
			pPoly->attr |= POLY4DV1_ATTR_SHADE_MODE_PURE;
			break;
		case PLG_SHADE_MODE_FLAT_FLAG:
			pPoly->attr |= POLY4DV1_ATTR_SHADE_MODE_FLAT;
			break;
		case PLG_SHADE_MODE_GOURAUD_FLAG:
			pPoly->attr |= POLY4DV1_ATTR_SHADE_MODE_GOURAUD;
			break;
		case PLG_SHADE_MODE_PHONG_FLAG:
			pPoly->attr |= POLY4DV1_ATTR_SHADE_MODE_PHONG;
			break;
		default:
			break;
		}
	}

	fclose(pFile);
	return 1;
}