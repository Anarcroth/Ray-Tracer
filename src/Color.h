#ifndef _COLOR_H
#define _COLOR_H

class Color {
	double red, green, blue, special;

public:

	Color();

	Color(const double &, const double &, const double &, const double &);

	//method functions
	double getColorRed();
	double getColorGreen();
	double getColorBlue();
	double getColorSpecial();

	void setColorRed(double &redValue);
	void setColorGreen(double &greenValue);
	void setColorBlue(double &blueValue);
	void setColorSpecial(double &specialValue);

	double brightness();

	Color colorScalar(double scalar);

	Color colorAdd(Color &color);

	Color colorMultiply(Color &color);

	Color colorAverage(Color &color);

	Color clip();

};

#endif
