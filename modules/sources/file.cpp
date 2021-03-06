#include "file.hpp"

int File::_id = 0;


std::string File::getName() const {
    return _name;
}

void File::setName(const std::string &name) {
    _name = name;
}

bool File::isEOF() const {
    return _isEOF;
}


int File::getFd() const {
    return _fd;
}

void File::setFd(int fd) {
    _fd = fd;
}

bool File::lock() {
    //TODO
}

bool File::unlock() {
    //TODO
}

File::File(bool local, std::string dir):_isLocal(local), _dir(dir) {
    _id++;
}

FileStatus File::create(std::string name, FileMode mode) {
    _mode = mode;
    std::string path = _dir + name;
    int fd = ::open(path.c_str(), O_RDWR|O_CREAT|O_TRUNC, _mode);
    if (fd < 0) {
        _status = FileStatus::CreateOperationFailed;
        return _status;
    } else {
        _fd = fd;
        _status = FileStatus::CreateOperationSuccess;
        return _status;
    }
}

FileStatus File::rcreate(std::string name, FileMode mode, UDPSocket server) {
    _isLocal = false;
    _name = name;
    setName(name);
    std::string str = "N: " + _name + " FM: " + std::to_string((int)mode);
    Message msg(str.c_str(), MessageType::Request, RPC::CreateFile);
    if (!_sock.sendMessageTo(server, msg)) {
        _status = FileStatus::CreateOperationFailed;
        return _status;
    } else {
        std::string serialized;
        if(_sock.recvFrom(server, serialized) == -1) {
            _status = FileStatus::CreateOperationFailed;
            return _status;
        } else {
            Message message = Message::deserialize(serialized);
            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {
                //TODO: Get FD from the Message;
                _status = FileStatus::CreateOperationSuccess;
                return _status;
            } else {
                _status = FileStatus::CreateOperationFailed;
                return _status;
            }
        }
    }
}

FileStatus File::open(std::string name, FileMode mode) {
    _isLocal = true;
    _mode = mode;
    std::string path = _dir + name;
//    std::cout << "Path: " << path << std::endl;
    int fd = ::open(path.c_str(), O_RDWR | O_APPEND, (int)_mode);
    if (fd < 0) {
        perror("Cannot Open File.");
        _status = FileStatus::OpenOperationFailed;
        return _status;
    } else {
        _fd = fd;
//        std::cout << "FD: " << fd << std::endl;
        _status = FileStatus::Opened;
        return _status;
    }
}

FileStatus File::ropen(std::string path, FileMode mode,  UDPSocket server) {
    std::cout << "Remote connection from " << _sock.getPortNumber() << std::endl;
    _isLocal = false;
    _name = path;
    std::string str = _name + " FM: " + std::to_string((int)mode);
    str[str.length()] = '\0';
    Message message(str.c_str(), MessageType::Request, RPC::OpenFile);
    if (!_sock.sendMessageTo(server, message)) {
        _status = FileStatus::OpenOperationFailed;
        return _status;
    } else {
        std::string serialized;
        if(_sock.recvFrom(server, serialized) == -1) {
            _status = FileStatus::OpenOperationFailed;
            return _status;
        } else {
            Message message = Message::deserialize(serialized);
            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {
                //TODO: Get FD from message;
                std::cout << "Opened!" << std::endl;
                _status = FileStatus::Opened;
                return _status;
            } else {
                std::cout << "============================Msg============================" << std::endl;
                std::cout << "Message Type Recieved: " << (int) message.getMessageType() << std::endl;
                std::cout << "Message Reply Type Recieved: " << (int) message.getReplyType() << std::endl;
                std::cout << "Message (Serialized) Recieved: " << serialized << std::endl;
                std::cout << "Message Recieved: " << message.getMessage() << std::endl;
                std::cout << "============================Msg============================" << std::endl;
                std::cout << "Failed!" << std::endl;
                _status = FileStatus::OpenOperationFailed;
                return _status;
            }
        }
    }
}

std::string File::read() {
    if (_status != FileStatus::Opened) {
        FileStatus status = open(_name, _mode);
        if(status != FileStatus::Opened) {
            perror("Cannot Open the File!");
            return "";
        }
    }
    int bytes, size = 0;
    char buffer[MAX_READ];
    bytes = ::read(_fd, &buffer, MAX_READ);
    if (bytes == -1) {
        perror("Error Reading From File!");
    } else if (bytes == 0) {
        _isEOF = true;
    } else {
        _isEOF = false;
        size = size + bytes;
    }
 /*   std::string strBuffer;
     while ((bytes = ::read(_fd, &buffer, MAX_READ))) {
        if (bytes == -1) {
            perror("Error Reading From File!");
        } else if (bytes == 0) {
            _isEOF = true;
        } else {
            size = size + bytes;
        }
        strBuffer = strBuffer + buffer;
    }
    if (bytes == -1) {
        perror("Error Reading From File!");
    } else if (bytes == 0) {
        _isEOF = true;
    }*/
    std::cout << "Bytes Read: " << bytes << std::endl;
    std::cout << "Size Read: " << size << std::endl;
    return std::string(buffer, size);
}

FileStatus File::rread(UDPSocket server) {
    std::cout << "Remote Read.." << std::endl;
    std::string str = _name + " FM: " + std::to_string((int)FileMode::ReadOnly);
    Message msg(str, MessageType::Request, RPC::ReadFile);
    if (!_sock.sendMessageTo(server, msg)) {
        _status = FileStatus::ReadOperationFailed;
        return _status;
    } else {
        std::cout << "Sending File succeed!" << std::endl;
        std::string serialized;
        if(_sock.recvFrom(server, serialized) == -1) {
            std::cout << "recvFrom Failed!" << std::endl;
            _status = FileStatus::ReadOperationFailed;
            return _status;
        } else {
            std::cout << "recvFrom succeed!" << std::endl;
            Message message = Message::deserialize(serialized);
            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {
                //TODO: Get buffer from message;
                _status = FileStatus::ReadOperationSuccess;
                //TODO: RETURN BUFFER;
                return _status;
            } else {
                //TODO: RETURN ERROR;
                _status = FileStatus::ReadOperationFailed;
                return _status;
            }
        }
    }
}

FileStatus File::write(std::string str) {
    _mode = FileMode::ReadWrite;
    if (_status != FileStatus::Opened) {
        FileStatus status = open(_name, _mode);
        if(status != FileStatus::Opened) {
            perror("Cannot Open the File!");
            return FileStatus::WriteOperationFailed;
        }
    }
    int bytes = ::pwrite(_fd, str.c_str(), str.length(), 0);
    if (bytes == -1) {
        perror("An error has occured while writing to the file.");
        return FileStatus::WriteOperationFailed;
    } else if (bytes == 0) {
        _isEOF = true;
    } else {
        _isEOF = false;
    }
    return FileStatus::WriteOperationSuccess;
}

FileStatus File::write(std::string str, unsigned int length) {
    _mode = FileMode::ReadWrite;
    if (_status != FileStatus::Opened) {
        FileStatus status = open(_name, _mode);
        if(status != FileStatus::Opened) {
            perror("Cannot Open the File!");
            return FileStatus::WriteOperationFailed;
        }
    }
    int bytes = ::pwrite(_fd, str.c_str(), length, 0);
    if (bytes == -1) {
        perror("An error has occured while writing to the file.");
        return FileStatus::WriteOperationFailed;
    } else if (bytes == 0) {
        _isEOF = true;
    }
    return FileStatus::WriteOperationSuccess;
}

FileStatus File::write(std::string name, std::string str, unsigned int length) {
    _name = name;
    return write(str, length);
}

FileStatus File::rwrite(std::string name, std::string txt, UDPSocket server) {
    _name = name;
    return rwrite(txt, server);
}

FileStatus File::rwrite(std::string txt, UDPSocket server) {
    std::string str = "N: " + _name + " FM: " + std::to_string((int)FileMode::ReadOnly) + " L: " + std::to_string(txt.length()) + " W: " + txt;
    Message msg(str, MessageType::Request, RPC::WriteToFile);
    if (!_sock.sendMessageTo(server, msg)) {
        _status = FileStatus::WriteOperationFailed;
        return _status;
    } else {
        std::string serialized;
        if(_sock.recvFrom(server, serialized) == -1) {
            _status = FileStatus::WriteOperationFailed;
            return _status;
        } else {
            Message message = Message::deserialize(serialized);
            if (message.getMessageType() == MessageType::Reply && message.getReplyType() == ReplyType::Success) {
                //TODO: Get buffer from message;
                _status = FileStatus::WriteOperationSuccess;
                //TODO: RETURN BUFFER;
                return _status;
            } else {
                //TODO: RETURN ERROR;
                _status = FileStatus::WriteOperationFailed;
                return _status;
            }
        }
    }
}

void File::setClientSocket(UDPSocket sock) {
     _sock = sock;
}

int File::getId() {
    return _id;
}

bool File::isLocal() const {
    return _isLocal;
}

bool File::isOpen() const {
    return _isOpen;
}

FileStatus File::getStatus() const {
    return _status;
}

FileMode File::getMode() const {
    return _mode;
}

void File::setMode(FileMode mode) {
    _mode = mode;
}

File::~File() {

}
