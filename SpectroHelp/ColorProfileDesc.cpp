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
		rtr(L"Оригинальное изображение без изменений", "Original image without any changes"))

		<< Temp(C, ModeTuned, rtr(L"Настроенное", "Tuned"),
			rtr(L"Три цветовых канала равномерное проецируются на два видимых",
			"Three color channels are projected into two visible"))

		//<< Temp(A, ModeShowAll, rtr(L"Все", "All"))
		//<< Temp(A, ModeRotateHsv, rtr(L"Вращать HSV", "Rotate HSV"))

		<< Temp(R, ModeFilter, rtr(L"Фильтр", "Filter"), rtr(L"Затемнить все цвета, кроме красного",	"Darken colors except red"))
		<< Temp(G, ModeFilter, rtr(L"Фильтр", "Filter"), rtr(L"Затемнить все цвета, кроме зеленого",	"Darken colors except green"))
		<< Temp(B, ModeFilter, rtr(L"Фильтр", "Filter"), rtr(L"Затемнить все цвета, кроме синего",		"Darken colors except blue"))

		<< Temp(R|G, ModeL_M, tr("R-G"),
			rtr(L"Красный минус зеленый - дает аналог восприятия красного цвета", "red minus green - gives analogue of red color sensation"))
		<< Temp(R|G, ModeRG2, tr("R-G/2"), rtr(L"Красный минус зеленый/2 - среднее между R-G и R", "red minus green/2 - average between R and R-G modes"))
		//<< Temp(C|A, ModeStrip, tr("Strip"), tr("Show strips with different channels"))

		<< Temp(R, ModeCone, tr("RRR"), rtr(L"Копировать значение красного канала во все остальные", "Copy red channel value to other channels"))
		<< Temp(G, ModeCone, tr("GGG"), rtr(L"Копировать значение зеленого канала во все остальные", "Copy green channel value to other channels"))
		<< Temp(B, ModeCone, tr("BBB"), rtr(L"Копировать значение синего канала во все остальные", "Copy blue channel value to other channels"))

		<< Temp(D, ModeR, tr("R.."), rtr(L"Оставить только красный канал", "Leave only red channel"))
		<< Temp(D, ModeG, tr(".G."), rtr(L"Оставить только зелеый канал", "Leave only green channel"))
		<< Temp(D, ModeB, tr("..B"), rtr(L"Оставить только синий канал", "Leave only blue channel"))

		<< Temp(R|G, ModeLM, tr("R+G"), rtr(L"Яркость = красный + зеленый (этот режим добавлен для учебных целей и демонстрации того, что синий цвет обычно мало влияет на восприятие яркости)",
			"Luminance = red + green (this mode is added for for educational purposes to demonstrate that blue color normally gives little impact into lightness perceiving)"))

		<< Temp(D, ModeImitateR, tr("No R"),
			rtr(L"Имитация отсутствия красного канала. Для сохранения восприятия яркости и белого цвета, значение зеленого канала скопировано в красный",
			"Imitated as no red color channel. To save lightness and white color appearance in this mode, red channel is filled with value from green channel."))
		<< Temp(D, ModeImitateG, tr("No G"),
			rtr(L"Имитация отсутствия зеленого канала. Для сохранения восприятия яркости и белого цвета, значение красного канала скопировано в зеленый",
			"Imitated as no green color channel. To save lightness and white color appearance in this mode, green channel is filled with value from red channel."))
		<< Temp(D, ModeImitateB, tr("No B"),
			rtr(L"Имитация отсутствия синего канала. Для сохранения восприятия яркости и белого цвета, значение зеленого канала скопировано в синий",
			"Imitated as no blue color channel. To save lightness and white color appearance in this mode, blue channel is filled with value from green channel."))

		<< Temp(C, ModeLM_S, tr("L+M-S"), rtr(L"(R+G)/2-B -> сделать серым",
			"Minor education-purpose mode: (R+G)/2-B -> make gray"))
		<< Temp(C, ModeRG_B, tr("R+G-B"), rtr(L"(R+G)/2-B -> RG", "(R+G)/2-B -> RG"))
		<< Temp(D, ModeImitateAll, tr("-RGB"), rtr(L"Имитация полной цветовой слепоты: яркость = красный + зеленый + немного синего",
			"Gray color = Luminance = red + green + little portion of blue"))
		;

	for(Temp t: li) {
		if(t.flags & R) liR << t.desc;
		if(t.flags & G) liG << t.desc;
		if(t.flags & B) liB << t.desc;
		if(t.flags & D) liD << t.desc;
	}
	int index = 0;
	//ret << new ColorProfileDesc(index++, ProfileAll, rtr(L"Все", "All"), rtr(L"Все цветовые каналы", "All color channels"), liA);
	ret << new ColorProfileDesc(index++, ProfileR, tr("R"), rtr(L"Red = Красный", "Red"), liR);
	ret << new ColorProfileDesc(index++, ProfileG, tr("G"), rtr(L"Green = зеленый", "Green"), liG);
	ret << new ColorProfileDesc(index++, ProfileB, tr("B"), rtr(L"Blue = синий", "Blue"), liB);
	ret << new ColorProfileDesc(index++, ProfileDemo, tr("Demo"), rtr(L"Демонстрационные режимы", "Demonstration modes"), liD);
	return ret;
}
int ColorProfileDesc::indexOfMode(Mode mode)const {
	int ret = INDEX_OF_EXPR(modes, it, mode==it.mode);
	return ret;
}