#ifndef _COLOR_H
#define _COLOR_H

class Color
{
 private:
	double red, green, blue, special;

public:
	Color();

	Color(const double, const double, const double, const double);

	double getColorRed() const;
	double getColorGreen() const;
	double getColorBlue() const;
	double getColorSpecial() const;

	void setColorRed(double redValue);
	void setColorGreen(double greenValue);
	void setColorBlue(double blueValue);
	void setColorSpecial(double specialValue);

	double brightness();

	Color colorScalar(double scalar);

	Color colorAdd(Color &color);

	Color colorMultiply(Color &color);

	Color colorAverage(Color &color);

	Color clip();

};

#endif
