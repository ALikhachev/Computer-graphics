#include "filter.h"
#include "utils.h"

FilterWorker::FilterWorker(Filter *filter, QImage image) :
  f(filter),
  image(image)
{
}

void FilterWorker::run() {
    emit progressChanged(0);
    emit imageReady(this->f->applyFilter(image, [this] (int progress) {
        emit progressChanged(progress);
    }));
    emit progressChanged(100);
}

void Filter::request() {
    emit requested(this);
}

FilterSettings *Filter::getSettings() {
    return NULL;
}

void Filter::setSettings(FilterSettings *) {
}
