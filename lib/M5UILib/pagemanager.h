#ifndef UILIB_PAGEMANAGER_H
#define UILIB_PAGEMANAGER_H

#include "page.h"
#include <stddef.h>

/**
 * @file pagemanager.h
 * @brief ページの遷移や登録、ループ更新を管理するページマネージャー名前空間の定義ヘッダーファイル
 */

namespace uilib {
    namespace PageManager {
        /**
         * @brief ページマネージャーにページを登録します。
         * @param page 登録するページへのポインタ
         * @param id ページの識別ID
         */
        void add(Page* page, size_t id);

        /**
         * @brief 現在アクティブなページの更新処理（update）を呼び出します。
         */
        void update();

        /**
         * @brief 指定したIDのページに遷移（アクティブ化）します。
         * @param id 遷移先ページのID
         */
        void set(size_t id);
    }
}

#endif // UILIB_PAGEMANAGER_H
