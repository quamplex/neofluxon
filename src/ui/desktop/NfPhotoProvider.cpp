
        QObject::connect(m_photoProvider, photosLoaded(const QList<NfPhotoId>& ids))
        // Callback for new photos
        m_photoProvider->setNewPhotosCallback([this](std::vector<std::unique_ptr<NfPhotoInfo>> newPhotos) {
                QMetaObject::invokeMethod(this, [this, newPhotos = std::move(newPhotos)]() mutable {

                        const int firstRow = static_cast<int>(m_photoIds.size());
                        const int count    = static_cast<int>(newPhotos.size());
                        const int lastRow  = firstRow + count - 1;

                        beginInsertRows(QModelIndex(), firstRow, lastRow);

                        m_photoIds.reserve(m_photoIds.size() + count);
                        std::ranges::for_each(newPhotos, [this](auto& ptr) {
                                m_photoIds.push_back(std::move(ptr));
                        });

                        // Update map: PhotoId -> QPersistentModelIndex
                        for (int i = 0; i < count; ++i) {
                                const auto& photoInfo = m_photoIds[firstRow + i];
                                m_itemsMap[photoInfo->id()] = QPersistentModelIndex(index(firstRow + i));
                        }

                        endInsertRows();
                }, Qt::QueuedConnection);
        });



        // Thumbnails ready callback
        m_photoProvider->setThumbnailsReadyCb([this](std::vector<NfThumbnail> thumbnails) {
                QMetaObject::invokeMethod(this, [this, thumbnails = std::move(thumbnails)]() mutable {
                }, Qt::QueuedConnection);
        });


QPixmap& NfBrowserModel::getThumbnail(const QModelIndex &index) const
{
    const auto& photoInfo = m_photos[index.row()];
    auto const *cacheImage = ;
    if (cacheImage) {
            auto const *thumbnail = dynamic_cast<const NfQtPixmap*>(cacheImage);
        if (thumbnail)
                return thumbnail->pixmap();
    }

    m_photoProvider->requestThumbnail(photoInfo, std::make_unique<NfQtPixmap>());

    return m_thumbnailPlaceholder;
}
