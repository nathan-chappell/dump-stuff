#include <iomanip>
#include <iostream>
#include <list>

using namespace std;

struct DrainingBufferBase
{
    string Pop()
    {
        auto result = _buffer.front();
        _buffer.pop_front();
        return result;
    }

    void Push(const string& message) { _buffer.push_back(message); }

    bool IsEmpty() { return _buffer.size() == 0; }

    virtual void Drain() = 0;

    virtual ~DrainingBufferBase() { }

private:
    list<string> _buffer {};
};

struct IMessenger
{
    virtual void SendMessage(const string& message) = 0;
    virtual void ReceiveMessage(const string& message) = 0;
    virtual ~IMessenger() { }
};

struct BufferedSenderBase : virtual IMessenger, private DrainingBufferBase
{
    virtual void SendMessage(const string& message) override { Push(message); }

    virtual void OnMessage(const string& message) { }

    void Drain() final
    {
        while (!IsEmpty()) {
            auto message = Pop();
            OnMessage(message);
            ReceiveMessage(message);
        }
    }
};

struct BufferedReceiverBase : virtual IMessenger, private DrainingBufferBase
{
    virtual void ReceiveMessage(const string& message) override { Push(message); }

    virtual void OnMessage(const string& message) { }

    void Drain() final
    {
        while (!IsEmpty()) {
            auto message = Pop();
            OnMessage(message);
        }
    }
};

namespace Mixins {

struct NotifyingMessageSender : virtual BufferedSenderBase
{
    virtual void OnMessage(const string& message) final
    {
        cout << "NotifyingMessageSender.OnMessage(" << message << ")" << endl;
    }
};

struct ImmediateMessageSender : virtual BufferedSenderBase
{
    virtual void SendMessage(const string& message) final
    {
        cout << "ImmediateMessageSender.SendMessage(" << message << ")" << endl;
        BufferedSenderBase::SendMessage(message);
        Drain();
    }
};

struct NotifyingMessageReceiver : virtual BufferedReceiverBase
{
    virtual void OnMessage(const string& message) final
    {
        cout << "NotifyingMessageReceiver.OnMessage(" << message << ")" << endl;
    }
};

struct ImmediateMessageReceiver : virtual BufferedReceiverBase
{
    virtual void ReceiveMessage(const string& message) final
    {
        cout << "ImmediateMessageReceiver.ReceiveMessage(" << message << ")" << endl;
        BufferedReceiverBase::ReceiveMessage(message);
        Drain();
    }
};

}

using namespace Mixins;

struct MyMessenger : NotifyingMessageSender,
                     ImmediateMessageSender,
                     NotifyingMessageReceiver,
                     ImmediateMessageReceiver
{
};

void Demo()
{
    MyMessenger myMessenger {};
    myMessenger.SendMessage("hello world");
}

int main(int argc, char** argv)
{
    cin >> setbase(16);
    Demo();
}