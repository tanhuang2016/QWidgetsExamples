#pragma once

#include "ControlRegistry.h"

class QApplication;
class TranslationManager;

class AppContext {
public:
    static AppContext &instance();

    void initialize(QApplication *app);
    ControlRegistry &registry();
    TranslationManager &translations();

private:
    AppContext();
    void registerPages();

    ControlRegistry m_registry;
    TranslationManager *m_translations;
    bool m_initialized;
};
