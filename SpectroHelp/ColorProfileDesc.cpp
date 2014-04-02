//ColorProfileDesc.cpp by Kostya Kozachuck as neurocod
#include "pch.h"
#include "ColorProfileDesc.h"

//static
ColorProfileDesc*ColorProfileDesc::get(ColorProfile cp) {
	for(ColorProfileDesc* d: allProfiles())
		if(d->cp==cp)
			return d;
	ASSERT(0);
	return allProfiles()[0];
}
//static
ColorProfileDesc*ColorProfileDesc::get(int index) {
	for(ColorProfileDesc* d: allProfiles())
		if(d->index==index)
			return d;
	ASSERT(0);
	return allProfiles()[0];
}
//static
QString ColorProfileDesc::modeDesc(ColorProfile cp, Mode m) {
	return get(cp)->mode(m).tooltip;
}
//static
QList<ColorProfileDesc*> ColorProfileDesc::allProfiles() {
	static QList<ColorProfileDesc*> ret;
	if(!ret.isEmpty())
		return ret;
	//flags of ColorProfile
	const int R = (1<<0);
	const int G = (1<<1);
	const int B = (1<<2);
	const int C = R|G|B;//c=color
	const int D = (1<<3);//demo
	QList<ModeDesc> liR, liG, liB, liD;
	struct Temp {
		ModeDesc desc;
		int flags;
		Temp(int _flags, Mode m, QString name, QString tool=""): flags(_flags),
			desc(m, name, tool) {}
	};
	QList<Temp> li;
	li
		<< Temp(C|D, ModeOriginal, rtr(L"=", "="),
		rtr(L"������������ ����������� ��� ���������", "Original image without any changes"))

		<< Temp(C, ModeTuned, rtr(L"�����������", "Tuned"),
			rtr(L"��� �������� ������ ����������� ������������ �� ��� �������",
			"Three color channels are projected into two visible"))

		//<< Temp(A, ModeShowAll, rtr(L"���", "All"))
		//<< Temp(A, ModeRotateHsv, rtr(L"������� HSV", "Rotate HSV"))

		<< Temp(R, ModeFilter, rtr(L"������", "Filter"), rtr(L"��������� ��� �����, ����� ��������",	"Darken colors except red"))
		<< Temp(G, ModeFilter, rtr(L"������", "Filter"), rtr(L"��������� ��� �����, ����� ��������",	"Darken colors except green"))
		<< Temp(B, ModeFilter, rtr(L"������", "Filter"), rtr(L"��������� ��� �����, ����� ������",		"Darken colors except blue"))

		<< Temp(R|G, ModeL_M, tr("R-G"),
			rtr(L"������� ����� ������� - ���� ������ ���������� �������� �����", "red minus green - gives analogue of red color sensation"))
		<< Temp(R|G, ModeRG2, tr("R-G/2"), rtr(L"������� ����� �������/2 - ������� ����� R-G � R", "red minus green/2 - average between R and R-G modes"))
		//<< Temp(C|A, ModeStrip, tr("Strip"), tr("Show strips with different channels"))

		<< Temp(R, ModeCone, tr("RRR"), rtr(L"���������� �������� �������� ������ �� ��� ���������", "Copy red channel value to other channels"))
		<< Temp(G, ModeCone, tr("GGG"), rtr(L"���������� �������� �������� ������ �� ��� ���������", "Copy green channel value to other channels"))
		<< Temp(B, ModeCone, tr("BBB"), rtr(L"���������� �������� ������ ������ �� ��� ���������", "Copy blue channel value to other channels"))

		<< Temp(D, ModeR, tr("R.."), rtr(L"�������� ������ ������� �����", "Leave only red channel"))
		<< Temp(D, ModeG, tr(".G."), rtr(L"�������� ������ ������ �����", "Leave only green channel"))
		<< Temp(D, ModeB, tr("..B"), rtr(L"�������� ������ ����� �����", "Leave only blue channel"))

		<< Temp(R|G, ModeLM, tr("R+G"), rtr(L"������� = ������� + ������� (���� ����� �������� ��� ������� ����� � ������������ ����, ��� ����� ���� ������ ���� ������ �� ���������� �������)",
			"Luminance = red + green (this mode is added for for educational purposes to demonstrate that blue color normally gives little impact into lightness perceiving)"))

		<< Temp(D, ModeImitateR, tr("No R"),
			rtr(L"�������� ���������� �������� ������. ��� ���������� ���������� ������� � ������ �����, �������� �������� ������ ����������� � �������",
			"Imitated as no red color channel. To save lightness and white color appearance in this mode, red channel is filled with value from green channel."))
		<< Temp(D, ModeImitateG, tr("No G"),
			rtr(L"�������� ���������� �������� ������. ��� ���������� ���������� ������� � ������ �����, �������� �������� ������ ����������� � �������",
			"Imitated as no green color channel. To save lightness and white color appearance in this mode, green channel is filled with value from red channel."))
		<< Temp(D, ModeImitateB, tr("No B"),
			rtr(L"�������� ���������� ������ ������. ��� ���������� ���������� ������� � ������ �����, �������� �������� ������ ����������� � �����",
			"Imitated as no blue color channel. To save lightness and white color appearance in this mode, blue channel is filled with value from green channel."))

		<< Temp(C, ModeLM_S, tr("L+M-S"), rtr(L"(R+G)/2-B -> ������� �����",
			"Minor education-purpose mode: (R+G)/2-B -> make gray"))
		<< Temp(C, ModeRG_B, tr("R+G-B"), rtr(L"(R+G)/2-B -> RG", "(R+G)/2-B -> RG"))
		<< Temp(D, ModeImitateAll, tr("-RGB"), rtr(L"�������� ������ �������� �������: ������� = ������� + ������� + ������� ������",
			"Gray color = Luminance = red + green + little portion of blue"))
		;

	for(Temp t: li) {
		if(t.flags & R) liR << t.desc;
		if(t.flags & G) liG << t.desc;
		if(t.flags & B) liB << t.desc;
		if(t.flags & D) liD << t.desc;
	}
	int index = 0;
	//ret << new ColorProfileDesc(index++, ProfileAll, rtr(L"���", "All"), rtr(L"��� �������� ������", "All color channels"), liA);
	ret << new ColorProfileDesc(index++, ProfileR, tr("R"), rtr(L"Red = �������", "Red"), liR);
	ret << new ColorProfileDesc(index++, ProfileG, tr("G"), rtr(L"Green = �������", "Green"), liG);
	ret << new ColorProfileDesc(index++, ProfileB, tr("B"), rtr(L"Blue = �����", "Blue"), liB);
	ret << new ColorProfileDesc(index++, ProfileDemo, tr("Demo"), rtr(L"���������������� ������", "Demonstration modes"), liD);
	return ret;
}
int ColorProfileDesc::indexOfMode(Mode mode)const {
	int ret = INDEX_OF_EXPR(modes, it, mode==it.mode);
	return ret;
}