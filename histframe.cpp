#include "histframe.h"

HistFrame::HistFrame(Cell* decidingCell, QQueue<char> values)
	: nextBestFrame(nullptr)
{
	char value = values.dequeue();
	newValues.insert(decidingCell, value);

	if( values.size() > 0 ){
		nextBestFrame = new HistFrame(decidingCell, values);
	}
}

HistFrame::HistFrame() : nextBestFrame(nullptr)
{

}
