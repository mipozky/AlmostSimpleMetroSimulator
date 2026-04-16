#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <string>
#include <streambuf>
#include <ostream>

//bool showfps;

class Console {
public:
    Console(tgui::Gui& gui) {
        panel = tgui::Panel::create({ "60%", "45%" });
        panel->getRenderer()->setBackgroundColor({ 30, 30, 30, 220 });
        panel->setVisible(false);
        gui.add(panel);

        history = tgui::TextArea::create();
        history->setSize({ "100%", "85%" });
        history->setReadOnly(true);
        history->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        history->getRenderer()->setTextColor(tgui::Color::White);
        panel->add(history);

        input = tgui::EditBox::create();
        input->setSize({ "100%", "15%" });
        input->setPosition(0, "85%");
        input->setDefaultText("Enter command...");
        panel->add(input);

        input->onReturnKeyPress([this]() {
            auto text = input->getText().toStdString();
            if (!text.empty()) {
                handleCommand(text);
                input->setText("");
            }
            });
    }
    Console() = default;
    void toggle() {
        panel->setVisible(!panel->isVisible());
        if (panel->isVisible()) input->setFocused(true);
    }
    void on(){
		panel->setVisible(true);
    }
	bool isVisible() const {
		return panel->isVisible();
	}
    void log(const std::string& msg) {
        history->addText(msg + "\n");
        if (history->getText().length() > 10000) {
            auto currentText = history->getText();
            history->setText(currentText.substr(5000));
        }
    }
    void log(const std::string& msg, const tgui::Color& color) {
        auto oldColor = history->getRenderer()->getTextColor();
        history->getRenderer()->setTextColor(color);
        log(msg);
        history->getRenderer()->setTextColor(oldColor);
        if (history->getText().length() > 6000) {
            auto currentText = history->getText();
            history->setText(currentText.substr(5000));
        }
	}
private:
    void handleCommand(const std::string& cmd) {
        try {
            log("> " + cmd);
            if (cmd == "clear") history->setText("");
            else if (cmd == "exit") exit(0);
            else if (cmd == "toggleFps") showfps = !showfps;
            else if (cmd == "toggleSimQuality") {simQuality = !simQuality;}
            else log("Unknown: " + cmd);
        }
        catch (const std::exception& e) {
            log(std::string("Error: ") + e.what(), tgui::Color::Red);
		}
    }

    tgui::Panel::Ptr    panel;
    tgui::TextArea::Ptr history;
    tgui::EditBox::Ptr  input;
};

class ConsoleBuf : public std::streambuf {
public:
    ConsoleBuf(Console& console, std::ostream& stream)
        : console_(console), stream_(stream), original_(stream.rdbuf(this))
    {
    }
    ConsoleBuf(Console& console, std::ostream& stream, bool error)
        : console_(console), stream_(stream), original_(stream.rdbuf(this))
    {
		this->error = error;
    }
    ~ConsoleBuf() {
        stream_.rdbuf(original_);
    }

protected:
    int overflow(int c) override {
        if (c == EOF) return EOF;
        buffer_ += static_cast<char>(c);
        if (c == '\n') flush_line();
        return c;
    }

    std::streamsize xsputn(const char* s, std::streamsize n) override {
        buffer_.append(s, n);
        auto pos = buffer_.rfind('\n');
        if (pos != std::string::npos) {
            flush_line();
        }
        return n;
    }

private:
    void flush_line() {
        
        if (!buffer_.empty() && buffer_.back() == '\n')
            buffer_.pop_back();
        if (!buffer_.empty())
            if(error) console_.log(buffer_, tgui::Color::Red);
            else console_.log(buffer_);
        buffer_.clear();
        
    }

    Console& console_;
    std::ostream& stream_;
    std::streambuf* original_;
    std::string     buffer_;
	bool error = false;
};