/**
 * File name: PhotoSourceLoader.cpp
 * Project: Neofluxon (a photography workflow software)
 *
 * Copyright (C) 2026 Iurie Nistor
 *
 * This file is part of Neofluxon.
 *
 * Neofluxon is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "PhotoSourceLoader.h"
#include "PhotoItem.h"
#include "PathWorker.h"
#include "PhotoWorker.h"
#include "Application.h"

#include <QThreadPool>

PhotoSourceLoader::PhotoSourceLoader(QObject* parent)
        : QObject(parent)
        , pathWorker{new PathWorker(this)}
        , loadRecursively{false}
        , threadPool{QThreadPool::globalInstance()}
        , isSetPath{false}
{

        threadPool->setMaxThreadCount(threadPool->maxThreadCount() + 1);
        LIQUIDFLOW_LOG_DEBUG() << "maxThreadCount: " << threadPool->maxThreadCount();

        pathWorker->setAutoDelete(false);
        QObject::connect(pathWorker,
                         &PathWorker::photoAvailable,
                         this,
                         &PhotoSourceLoader::photoAvailable);
}

PhotoSourceLoader::~PhotoSourceLoader()
{
        pathWorker->stop();
        threadPool->clear();
        NeofluxonApplication::releaseGuiSemaphore();
        threadPool->waitForDone();
}

void PhotoSourceLoader::stop()
{
        pathWorker->stop();
}

PhotoSource* PhotoSourceLoader::getSource() const
{
        return photoSource;
}

void PhotoSourceLoader::setSource(const PhotoSource* source)
{
        photoSource = srd::move(std::unique_ptr<decltype(source)>(source));
        if (isSetSource)
                return;
        
        pathWorker->stop();
        threadPool->clear();
        NeofluxonApplication::releaseGuiSemaphore();

        lflow_debug() << "wait for done...";
        threadPool->waitForDone();

        lflow_debug() << "process events...";
        NeofluxonApplication::processEvents();
        lflow_debug() << "process events done";

        emit pathChanged(rootPath);

        lflow_debug() << "set root path: " << rootPath;
        pathWorker->setPath(rootPath);
        threadPool->start(pathWorker);
}

void PhotoSourceLoader::setRecursively(bool b)
{
        loadRecursively = b;
}
