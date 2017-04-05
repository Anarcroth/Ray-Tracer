#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <omp.h>

#include <stdlib.h>
#include <time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Source.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

using namespace std;

struct RGBType {
	double r;
	double g;
	double b;
};

struct SceneObjects
{
	static Sphere scene_sphere;
	static Sphere scene_sphere2;
	static Plane scene_plane;
	static Plane scene_plane2;
	static Plane scene_plane3;
	static Plane scene_plane4;
	static Plane scene_plane5;

	static Light scene_light;

	vector<Source*> light_sources;

	vector<Object*> scene_objects;
} SceneObjectsCall;

struct VectorPosition
{
	const static Vect O;
	const static Vect X;
	//Y has to be a static without a const because it is used in DefineCamOrientation() as a parameter
		  static Vect Y;
	const static Vect P;
	const static Vect R;
	const static Vect M;
	const static Vect NEW_SPHERE_LOCATION;
	const static Vect LIGHT_POSITION;
		  static Vect look_at;
		  static Vect diff_btw;
		  static Vect campos;
		  static Vect camdir;
		  static Vect camright;
		  static Vect camdown;
		  static Camera scene_cam;
		  static Vect intersecting_ray_direction;
} VectPositionCall;

struct ColorOfScene
{
	const static Color WHITE_LIGHT;
	const static Color PRETTY_GREEN;
	const static Color MAROON;
	const static Color TILE_FLOOR;
	const static Color GRAY;
	const static Color BLACK;
		  static Color intersection_color;

} ColorsOfSceneCall;

struct CameraRay
{
	static Vect cam_ray_origin;
} CameraRayCall;

class RayTrace
{
public:
	
	void BeginRender();
	void AddColorToPixels();
	void DefineCamOrientation();
	void VectorSceneDefinitions();
	void AntiAliasingCalc();
	void CamRayCalc();
	void SetBackgroundBlack();
	void ColorInPoints();
	void SaveBMPImage(const char *filename, RGBType* data);
	int WinningObjectIndex(vector<double> object_intersections);
	Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight);

private:

	const double SCREEN_WIDTH = 1000;
	const double SCREEN_HEIGHT = 800;
	//AADEPTH sends out n number of pixels/rays that hit one pixels and then reflect to other pixels are return a value that is then averaged
	//the bigger the AADEPTH the more calculations are made, the clearer the image
	const static int AADEPTH = 4;

	int n = (int)SCREEN_WIDTH*(int)SCREEN_HEIGHT;
	RGBType *pixels = new RGBType[n];
	
	double aspectratio = SCREEN_WIDTH / SCREEN_HEIGHT;
	const double AMBIEN_TLIGHT = 0.2;
	const double ACCURACY = 0.000001;

	int thisone, aa_index;
	
	int x, y, aax, aay;

	//start with a blank pixel
	double tempRed[AADEPTH*AADEPTH];
	double tempGreen[AADEPTH*AADEPTH];
	double tempBlue[AADEPTH*AADEPTH];

	double zero = 0, one = 1;

	int index_of_winning_object = 0;

	double xamnt, yamnt;

};

const Vect VectorPosition::O(0, 0, 0);
const Vect VectorPosition::X(3, 0, 3.5);
	  Vect VectorPosition::Y(0, 1, 0);
const Vect VectorPosition::P(5.5, 0, 3.5);
const Vect VectorPosition::R(0, -3, 0);
const Vect VectorPosition::M(7, 0, -5);
const Vect VectorPosition::LIGHT_POSITION(-7, 10, -10);
const Vect VectorPosition::NEW_SPHERE_LOCATION(2, 0, 0);

Vect VectorPosition::campos(4, 1, -4);
Vect VectorPosition::camdir;
Vect VectorPosition::camright;
Vect VectorPosition::camdown;

Camera VectorPosition::scene_cam(campos, camdir, camright, camdown);

Vect VectorPosition::look_at(0, 0, 0);
Vect VectorPosition::diff_btw(campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());

const Color ColorOfScene::WHITE_LIGHT(1.0, 1.0, 1.0, 0);
const Color ColorOfScene::PRETTY_GREEN(0.25, 0.45, 0.95, 0.5);
const Color ColorOfScene::MAROON(0.95, 0.5, 0.5, 0.5);
const Color ColorOfScene::TILE_FLOOR(1, 1, 1, 2);
const Color ColorOfScene::GRAY(0.5, 0.5, 0.5, 0);
const Color ColorOfScene::BLACK(0.0, 0.0, 0.0, 0);
Color ColorOfScene::intersection_color;

Sphere SceneObjects::scene_sphere(VectPositionCall.O, 0.85, ColorsOfSceneCall.PRETTY_GREEN);
Sphere SceneObjects::scene_sphere2(VectPositionCall.NEW_SPHERE_LOCATION, 0.5, ColorsOfSceneCall.MAROON);

Plane SceneObjects::scene_plane(VectPositionCall.Y, -1, ColorsOfSceneCall.TILE_FLOOR);
Plane SceneObjects::scene_plane2(VectPositionCall.P, -1, ColorsOfSceneCall.GRAY);
Plane SceneObjects::scene_plane3(VectPositionCall.X, 1, ColorsOfSceneCall.GRAY);
Plane SceneObjects::scene_plane4(VectPositionCall.R, -1, ColorsOfSceneCall.TILE_FLOOR);
Plane SceneObjects::scene_plane5(VectPositionCall.M, -1, ColorsOfSceneCall.BLACK);

Light SceneObjects::scene_light(VectPositionCall.LIGHT_POSITION, ColorsOfSceneCall.WHITE_LIGHT);

Vect CameraRay::cam_ray_origin;

Vect VectorPosition::intersecting_ray_direction;

void RayTrace::SaveBMPImage(const char *filename, RGBType* data) {

	//file creating name and function
	FILE *f;
	int k = (int)SCREEN_WIDTH * (int)SCREEN_HEIGHT;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int dpi = 72;
	int ppm = dpi*m;

	//creating a file by hexadecimal code??
	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(SCREEN_WIDTH);
	bmpinfoheader[5] = (unsigned char)((int)SCREEN_WIDTH >> 8);
	bmpinfoheader[6] = (unsigned char)((int)SCREEN_WIDTH >> 16);
	bmpinfoheader[7] = (unsigned char)((int)SCREEN_WIDTH >> 24);

	bmpinfoheader[8] = (unsigned char)(SCREEN_HEIGHT);
	bmpinfoheader[9] = (unsigned char)((int)SCREEN_HEIGHT >> 8);
	bmpinfoheader[10] = (unsigned char)((int)SCREEN_HEIGHT >> 16);
	bmpinfoheader[11] = (unsigned char)((int)SCREEN_HEIGHT >> 24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s >> 8);
	bmpinfoheader[23] = (unsigned char)(s >> 16);
	bmpinfoheader[24] = (unsigned char)(s >> 24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm >> 8);
	bmpinfoheader[27] = (unsigned char)(ppm >> 16);
	bmpinfoheader[28] = (unsigned char)(ppm >> 24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm >> 8);
	bmpinfoheader[31] = (unsigned char)(ppm >> 16);
	bmpinfoheader[32] = (unsigned char)(ppm >> 24);

	f = fopen(filename, "wb");

	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);

	//for each pixel in the window we set a RGB data information
	for (int i = 0; i < k; i++) {
		RGBType rgb = pixels[i];

		double red = (pixels[i].r) * 255;
		double green = (pixels[i].g) * 255;
		double blue = (pixels[i].b) * 255;

		unsigned char color[3] = { (int)blue,(int)green,(int)red };

		fwrite(color, 1, 3, f);
	}
}

void RayTrace::VectorSceneDefinitions()
{
	SceneObjectsCall.light_sources.push_back(dynamic_cast<Source*>(&SceneObjectsCall.scene_light));

	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_sphere));
	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_sphere2));
	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_plane));
	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_plane2));
	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_plane3));
	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_plane4));
	SceneObjectsCall.scene_objects.push_back(dynamic_cast<Object*>(&SceneObjectsCall.scene_plane5));
}

void RayTrace::AntiAliasingCalc()
{
	if (SCREEN_WIDTH > SCREEN_HEIGHT)
	{
		//the image is wider than it is tall
		xamnt = ((x + (double)aax / (AADEPTH - 1)) / SCREEN_WIDTH)*aspectratio - (((SCREEN_WIDTH - SCREEN_HEIGHT) / SCREEN_HEIGHT) / 2);
		yamnt = ((SCREEN_HEIGHT - y) + (double)aax / (AADEPTH - 1)) / SCREEN_HEIGHT;
	}
	else if (SCREEN_HEIGHT > SCREEN_WIDTH)
	{
		//the image is taller than it is wide
		xamnt = (x + (double)aax / (AADEPTH - 1)) / SCREEN_WIDTH;
		yamnt = (((SCREEN_HEIGHT - y) + (double)aax / (AADEPTH - 1)) / SCREEN_HEIGHT) / aspectratio - (((SCREEN_HEIGHT - SCREEN_WIDTH) / SCREEN_WIDTH) / 2);
	}
	else
	{
		//the image is square
		xamnt = (x + (double)aax / (AADEPTH - 1)) / SCREEN_WIDTH;
		yamnt = ((SCREEN_HEIGHT - y) + (double)aax / (AADEPTH - 1)) / SCREEN_HEIGHT;
	}
}

void RayTrace::CamRayCalc()
{
	CameraRayCall.cam_ray_origin = VectPositionCall.scene_cam.getCameraPosition();
}

void RayTrace::SetBackgroundBlack()
{
	tempRed[aa_index] = 0;
	tempGreen[aa_index] = 0;
	tempBlue[aa_index] = 0;
}

void RayTrace::ColorInPoints()
{
	tempRed[aa_index] = ColorsOfSceneCall.intersection_color.getColorRed();
	tempGreen[aa_index] = ColorsOfSceneCall.intersection_color.getColorGreen();
	tempBlue[aa_index] = ColorsOfSceneCall.intersection_color.getColorBlue();
}

void RayTrace::BeginRender()
{
	DefineCamOrientation();

	VectorSceneDefinitions();

	// using OMP to multithread the process
	//8 seems to be the optimal number of threads wuthout distroying the picture
	omp_set_num_threads(8);
	#pragma omp parallel

	for (x = 0; x < SCREEN_WIDTH; x++) {
		for (y = 0; y < SCREEN_HEIGHT; y++) {

			thisone = y*(int)SCREEN_WIDTH + x;

			for (aax = 0; aax < AADEPTH; aax++) {
				for (aay = 0; aay < AADEPTH; aay++) {

					aa_index = aay*AADEPTH + aax;

					AntiAliasingCalc();

					CamRayCalc();

					Vect cam_ray_direction = VectPositionCall.camdir.vectAdd(VectPositionCall.camright.vectMult(xamnt - 0.5).vectAdd(VectPositionCall.camdown.vectMult(yamnt - 0.5))).normalize();

					Ray cam_ray(CameraRayCall.cam_ray_origin, cam_ray_direction);

					vector<double> intersections;

					for (size_t index = 0; index < SceneObjectsCall.scene_objects.size(); index++)
					{
						intersections.push_back(SceneObjectsCall.scene_objects.at(index)->findIntersection(cam_ray));
					}

					index_of_winning_object = WinningObjectIndex(intersections);

					if (index_of_winning_object == -1) 
					{
						SetBackgroundBlack();
					}
					else 
					{
						//index corresponds to an object in the scene
						if (intersections.at(index_of_winning_object) > ACCURACY)
						{
							//determine the position and direction vectors at the point of intersection

							Vect intersection_position = CameraRayCall.cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
							VectPositionCall.intersecting_ray_direction = cam_ray_direction;

							//color in the points
							ColorsOfSceneCall.intersection_color = getColorAt(intersection_position,
								VectPositionCall.intersecting_ray_direction,
								SceneObjectsCall.scene_objects, index_of_winning_object,
								SceneObjectsCall.light_sources, ACCURACY, AMBIEN_TLIGHT);

							ColorInPoints();
						}
					}
				}
			}
			AddColorToPixels();
		}
	}
	SaveBMPImage("scene_anti-aliased_Ray_Trac11.0.bmp", pixels);
}

void RayTrace::DefineCamOrientation()
{
	VectPositionCall.camdir = VectPositionCall.diff_btw.negative().normalize();
	VectPositionCall.camright = VectPositionCall.Y.crossProduct(VectPositionCall.camdir).normalize();
	VectPositionCall.camdown = VectPositionCall.camright.crossProduct(VectPositionCall.camdir);
}

void RayTrace::AddColorToPixels()
{
	//this has to stay here
	//average the pixel color
	double totalRed = 0;
	double totalGreen = 0;
	double totalBlue = 0;

	for (int iRed = 0; iRed < AADEPTH*AADEPTH; iRed++) {
		totalRed = totalRed + tempRed[iRed];
	}

	for (int iGreen = 0; iGreen < AADEPTH*AADEPTH; iGreen++) {
		totalGreen = totalGreen + tempGreen[iGreen];
	}

	for (int iBlue = 0; iBlue < AADEPTH*AADEPTH; iBlue++) {
		totalBlue = totalBlue + tempBlue[iBlue];
	}

	double avgRed = totalRed / (AADEPTH*AADEPTH);
	double avgGreen = totalGreen / (AADEPTH*AADEPTH);
	double avgBlue = totalBlue / (AADEPTH*AADEPTH);

	pixels[thisone].r = avgRed;
	pixels[thisone].g = avgGreen;
	pixels[thisone].b = avgBlue;
}

int RayTrace::WinningObjectIndex(vector<double> object_intersections) {
	//return the index of the winning intersection
	int index_of_minimum_value;

	//prevent unnessary calculations
	if (object_intersections.size() == 0) {
		//if there are no intersections
		return -1;
	}
	else if (object_intersections.size() == 1) {
		//if there is one intersections
		if (object_intersections.at(0) > 0) {
			//if that intersection is greater than zero the its our index of minimum value
			//0 is the index of our value
			return 0;
		}
		else {
			//otherwise the only intersections value is negative
			return -1;
		}
	}
	else {
		//otherwise there is more than one intersection
		//first find the maximum value

		double max = 0;
		for (size_t i = 0; i < object_intersections.size(); i++) {
			if (max < object_intersections.at(i)) {
				max = object_intersections.at(i);
			}
		}

		//then starting from the maximum value find the minimum position value
		if (max > 0) {
			//we only want positive intersections
			for (size_t index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;
		}
		else {
			//all the intersections were negative
			return -1;
		}
	}
}

Color RayTrace::getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight) {

	Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	if (winning_object_color.getColorSpecial() == 2) {
		//checkered tile floor pattern

		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

		if ((square % 2) == 0) {
			//black tile
			winning_object_color.setColorRed(zero);
			winning_object_color.setColorGreen(zero);
			winning_object_color.setColorBlue(zero);
		}
		else {
			//white tile
			winning_object_color.setColorRed(one);
			winning_object_color.setColorGreen(one);
			winning_object_color.setColorBlue(one);
		}
	}

	Color final_color = winning_object_color.colorScalar(ambientlight);

	if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
		//reflection from object with specular intensity
		double dot1 = winning_object_normal.dotProduct(VectPositionCall.intersecting_ray_direction.negative());
		Vect scalar1 = winning_object_normal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(VectPositionCall.intersecting_ray_direction);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = VectPositionCall.intersecting_ray_direction.negative().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray(intersection_position, reflection_direction);

		//determine what the ray intersects with first
		vector<double> reflection_intersections;

		for (size_t reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) {
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
		}

		int index_of_winning_object_with_reflection = WinningObjectIndex(reflection_intersections);

		if (index_of_winning_object_with_reflection != -1) {
			//reflection ray missed everything else
			if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
				//determine the poition and direction at the point of intersection with the reflection
				//the ray only affects the color if it reflected off something
				Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;

				Color reflection_intersection_color = getColorAt(reflection_intersection_position,
																	reflection_intersection_ray_direction,
																		scene_objects,
																			index_of_winning_object_with_reflection,
																				light_sources,
																					accuracy,
																						ambientlight);


				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
			}
		}
	}

	for (size_t light_index = 0; light_index < light_sources.size(); light_index++) {
		Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();

		double cosine_angle = winning_object_normal.dotProduct(light_direction);

		if (cosine_angle > 0) {
			//test for shadows
			bool shadowed = false;

			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
			double distance_to_light_magnitude = distance_to_light.magnitude();

			Ray shadow_ray(intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());

			vector<double> secondary_intersections;

			for (size_t object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}

			for (size_t c = 0; c < secondary_intersections.size(); c++) {
				if (secondary_intersections.at(c) > accuracy) {
					if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
						shadowed = true;
					}
					break;
				}
			}

			if (shadowed == false) {
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

				if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1) {
					//special value reffers to shinyness
					double dot1 = winning_object_normal.dotProduct(VectPositionCall.intersecting_ray_direction.negative());
					Vect scalar1 = winning_object_normal.vectMult(dot1);
					Vect add1 = scalar1.vectAdd(VectPositionCall.intersecting_ray_direction);
					Vect scalar2 = add1.vectMult(2);
					Vect add2 = VectPositionCall.intersecting_ray_direction.negative().vectAdd(scalar2);
					Vect reflection_direction = add2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);
					if (specular > 0) {
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
					}
				}
			}
		}
	}

	return final_color.clip();
}

int main(int argc, char *argv[]) {

	RayTrace RendImage;

	clock_t t1, t2;
	t1 = clock();

	srand(time(0));

	RendImage.BeginRender();

	t2 = clock();
	double diff = ((double)t2 - (double)t1) / 1000;

	std::cout << diff << " seconds" << endl;

	return 0;
}
