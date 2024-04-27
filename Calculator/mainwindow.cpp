#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->display1->setPlainText("0");

    // Membuat fungsi perulangan btn 0-9
    for (int i = 0; i <= 9; ++i) {
        QString buttonName = "btn" + QString::number(i);
        QPushButton* button = findChild<QPushButton*>(buttonName);

    connect(button, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    }

    // Menghubungkan tombol operator ke slot onOperatorButtonClicked
    connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(onOperatorButtonClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(onOperatorButtonClicked()));
    connect(ui->btnMultiply, SIGNAL(clicked()), this, SLOT(onOperatorButtonClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(onOperatorButtonClicked()));

    // Menghubungkan tombol lainnya ke slot yang sesuai
    connect(ui->btnEquals, SIGNAL(clicked()), this, SLOT(onEqualsButtonClicked()));
    connect(ui->btnComa, SIGNAL(clicked()), this, SLOT(onCommaButtonClicked()));
    connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(onClearButtonClicked()));
    connect(ui->btnDelete, SIGNAL(clicked()), this, SLOT(onDeleteButtonClicked()));
    connect(ui->btnChangeSign, SIGNAL(clicked()), this, SLOT(onChangeSignButtonClicked()));
    connect(ui->btnPercen, SIGNAL(clicked()), this, SLOT(onPercenButtonClicked()));

    // Menghubungkan display1 ke display2
    connect(ui->display1->document(), SIGNAL(contentsChanged()), this, SLOT(updateDisplay2()));

}

MainWindow::~MainWindow() {
    delete ui;
}

// Display 2 akan menampilkan hasil dari perhitungan display 1
void MainWindow::updateDisplay2() {
    QString expression = ui->display1->toPlainText();
    QScriptValue result = engine.evaluate(expression);

    //Apakah ada ekspresi lain selain di program?, maka set ("  ")
    if (engine.hasUncaughtException()) {
        ui->display2->setPlainText(" ");
    }
    //Menampilkan ui-> display2, dan menunjukkan hasil perhitungan
    else {
        ui->display2->setPlainText(result.toString());
    }
}

// Membuat tombol clear yang akan menghapus nilai display1 dan display2
void MainWindow::onClearButtonClicked() {
    ui->display1->clear();  // Clear display1
    ui->display2->clear();  // Clear display2
}

// Fungsi Penampil Angka 0-9
void MainWindow::onNumberButtonClicked() {
    // Mencoba mengambil pointer ke tombol yang memicu sinyal yang terhubung ke slot ini
    QPushButton *button = qobject_cast<QPushButton *>(sender());

    if (button) {
        QString buttonText = button->text();
        QString currentText = ui->display1->toPlainText();

        if (currentText == "0") {
            // Jika teks saat ini adalah "0", gantilah dengan input pertama
            ui->display1->setPlainText(buttonText);
        } else {
            // Jika bukan "0", tambahkan digit ke teks yang ada
            ui->display1->insertPlainText(buttonText);
        }

        QTextCursor cursor = ui->display1->textCursor();
        cursor.movePosition(QTextCursor::End); // Memindahkan kursor ke akhir teks
        ui->display1->setTextCursor(cursor);
    }
}

//Fungsi Operator + - * /
void MainWindow::onOperatorButtonClicked() {
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) {
        return;
    }
    QString buttonText = button->text();
    QString currentText = ui->display1->toPlainText();

    // Memeriksa apakah teks saat ini kosong atau jika karakter terakhir bukan operator
    if (currentText.isEmpty() || (currentText.right(1) != "+" && currentText.right(1) != "-" && currentText.right(1) != "*" && currentText.right(1) != "/")) {
        ui->display1->insertPlainText(buttonText);
    }
}

// Fungsi Tombol koma ( , )
void MainWindow::onCommaButtonClicked() {
    QString currentText = ui->display1->toPlainText();

    ui->display1->insertPlainText(".");
}

//Fungsi Tombol Delete
void MainWindow::onDeleteButtonClicked() {
    QString currentText = ui->display1->toPlainText();

    if (!currentText.isEmpty()) {
        // Hapus 1 karakter terakhir
        currentText.chop(1);
        ui->display1->setPlainText(currentText);

        // Pindahkan cursor ke setelah teks sekarang
        QTextCursor cursor = ui->display1->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->display1->setTextCursor(cursor);
    }
}

void MainWindow::onChangeSignButtonClicked() {
    QString currentText = ui->display1->toPlainText();

    // Mencari angka terakhir dalam teks
    QRegExp rx("(\\b\\d+\\.?\\d*\\b)");
    int pos = rx.lastIndexIn(currentText);
    if (pos != -1) {
        QString lastMatched = rx.cap(1);
        QString replacement = "(-" + lastMatched + ")";

        // Mengganti angka terakhir dengan tanda negatif
        currentText.replace(pos, lastMatched.length(), replacement);

        // Menetapkan kursor setelah tanda kurung penutup
        pos += replacement.length();

        // Menetapkan teks yang telah dimodifikasi ke widget
        ui->display1->setPlainText(currentText);

        // Menetapkan posisi kursor
        QTextCursor cursor = ui->display1->textCursor();
        cursor.setPosition(pos);
        ui->display1->setTextCursor(cursor);
    }
}

void MainWindow::onPercenButtonClicked() {
    QString currentText = ui->display1->toPlainText();
    QRegExp rx("(\\d+\\.?\\d*)");

    int pos = 0;
    int lastPos = -1;
    QString lastMatched;

    while ((pos = rx.indexIn(currentText, pos)) != -1) {
        lastMatched = rx.cap(1);
        lastPos = pos;
        pos += rx.matchedLength();
    }

    if (lastPos != -1 && !lastMatched.isEmpty()) {
        double percentage = lastMatched.toDouble() / 100.0;
        currentText.replace(lastPos, lastMatched.length(), QString::number(percentage));
        int newPos = lastPos + QString::number(percentage).length();

        ui->display1->setPlainText(currentText);

        // Set the cursor position after the last replacement
        QTextCursor cursor = ui->display1->textCursor();
        cursor.setPosition(newPos);
        ui->display1->setTextCursor(cursor);
    }
}

void MainWindow::onEqualsButtonClicked() {
    QString expression = ui->display1->toPlainText();
    QScriptValue result = engine.evaluate(expression);

    // Mengatur hasil perhitungan ke display1
    ui->display1->setPlainText(result.toString());

    // Mengosongkan display2
    ui->display2->clear();

    // Menggeser kursor ke nilai selanjutnya
    QTextCursor cursor = ui->display1->textCursor();
    cursor.movePosition(QTextCursor::End); // Memindahkan kursor ke akhir teks
    ui->display1->setTextCursor(cursor);
}

