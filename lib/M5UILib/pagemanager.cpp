#include "pagemanager.h"
#include <map>

namespace uilib {
    namespace PageManager {
        namespace impl {
            size_t index = 0;
            std::map<size_t, Page*> pages;
        }

        void add(Page* page, size_t id) {
            impl::pages[id] = page;
        }

        void update() {
            if (impl::pages.find(impl::index) != impl::pages.end()) {
                impl::pages[impl::index]->update();
            }
        }

        void set(size_t id) {
            if (impl::pages.find(impl::index) != impl::pages.end()) {
                impl::pages[impl::index]->setActive(false);
            }
            impl::index = id;
            if (impl::pages.find(id) != impl::pages.end()) {
                impl::pages[id]->load();
            }
        }
    }
}
