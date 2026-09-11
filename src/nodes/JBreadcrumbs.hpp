#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QPushButton>

class JBreadcrumbs : public QWidget {
public:
    JBreadcrumbs(QWidget *parent);
    ~JBreadcrumbs();
    void addPage(QWidget * crumb);
    void removePage(int index);

private:
    std::unique_ptr<QVBoxLayout> layout;

    QScrollArea *scroll_area;
    QHBoxLayout *hbox_layout;
    std::vector<QWidget *> pages;
    std::vector<QWidget *> buttons;
};
