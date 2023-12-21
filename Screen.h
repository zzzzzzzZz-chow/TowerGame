#pragma once

#include "Object.h"
#include "Position.h"
#include <Windows.h>
#include <thread>
#include <mutex>
#include <map>
#include <iostream>
#include <stdio.h>

class Icon : public virtual Object, public virtual PointType {
	using Type = Icon;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	Icon() : m_data({ L"s" }) {}
	Icon(std::vector<std::wstring> data) : m_data(data) {}
	Icon(ConstReference that) = default;
	Icon(RightReference that) = default;
	Reference operator=(ConstReference that) = default;
	Reference operator=(RightReference that) = default;
	~Icon() = default;

	std::vector<std::wstring>& data() {
		return m_data;
	}
	const std::vector<std::wstring>& data() const {
		return m_data;
	}
private:
	std::vector<std::wstring> m_data{ {L"空"} };
};

class Screen : public virtual Object, public virtual PositionType {
	using Type = Screen;
	using Reference = Type & ;
	using ConstReference = const Type &;
	using RightReference = Type && ;
	using Pointer = Type * ;
	using ConstPointer = const Type *;
public:
	Screen() :
		m_startGamePosition(Position2D{ 0, 0 }),
		m_endGamePosition(Position2D{ 20000, 20000 })
	{
		
		m_pBuffer = new char[1000 * 2];
		std::memset(m_pBuffer, ' ', 1000);
		screenThread();
		//m_screenThread = std::thread(&Screen::screenThreadTask, this);
	}
	Screen(ConstReference that) = delete;
	Screen(RightReference that) = default;
	Reference operator=(ConstReference that) = delete;
	Reference operator=(RightReference that) = default;

	void show(const ID& id, const Position2D& position, const Icon& str) { // 把 在屏幕上的位子和图标保存到ID上
		Number x = position.x() * windowSize().x() / (m_endGamePosition.x() - m_startGamePosition.x());
		Number y = position.y() * windowSize().y() / (m_endGamePosition.y() - m_startGamePosition.y());
		
		std::lock_guard<std::mutex> lock{screenMutex()};
		if (x >= windowSize().x() || x < 0 || y >= windowSize().y() || y < 0) {
			if (objectPointInfo().find(id) != objectPointInfo().end())
				objectPointInfo().erase(id);
			return;
		}
		objectPointInfo()[id] = std::make_pair(Point2D{ x, y }, Icon{ str });
	}
	char* m_pBuffer;

	Position2D& startGamePosition() { return m_startGamePosition; }
	Position2D& endGamePosition() { return m_endGamePosition; }
protected:
	static std::mutex& screenMutex() {
		static std::mutex s_screenMutex{};
		return s_screenMutex;
	}

	static std::thread& screenThread() {
		static std::thread s_screenThread{Type::screenThreadFunction};
		return s_screenThread;
	}

	static void screenThreadFunction() {
		while (1) {
			std::lock_guard<std::mutex> lock{screenMutex()};
			for (int i = 0; i < windowSize().x() * windowSize().y(); ++i) {
				pWindowData()[i].Char.UnicodeChar = ' ';
				pWindowData()[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			}
			for (auto& pair : objectPointInfo()) {
				const ID& id = pair.first;
				Point2D& point = pair.second.first;
				Icon& icon = pair.second.second;
				std::vector<std::wstring>& data = icon.data();
				for (int y = 0; y < data.size(); ++y) {
					for (int x = 0; x < data[y].size(); ++x) {
						if (point.x() + x >= windowSize().x())
							break;
						pWindowData()[(point.y() + y) * windowSize().x() + point.x() + x].Char.UnicodeChar = data[y][x];
					}
					if ((point.y() + y) >= windowSize().y())
						break;
				}
			}
			static SMALL_RECT m_rect;
			m_rect.Top = 0;
			m_rect.Left = 0;
			m_rect.Bottom = SHORT(windowSize().y());
			m_rect.Right = SHORT(windowSize().x());
			static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			//std::terminate();
			WriteConsoleOutputW(handle, pWindowData(), COORD{ SHORT(windowSize().x()), SHORT(windowSize().y()) }, COORD{ 0, 0 }, &m_rect);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	void screenThreadTask() {
		while (1) {
			//for (int i = 0; i < m_windowSize.x() * m_windowSize.y(); ++i) {
			//	m_pWindowData[i].Char.AsciiChar = m_pBuffer[i];
			//	//m_pBuffer[i] = ' ';
			//	m_pWindowData[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			//}
			//static SMALL_RECT m_rect;
			//m_rect.Top = 0;
			//m_rect.Left = 0;
			//m_rect.Bottom = SHORT(m_windowSize.y());
			//m_rect.Right = SHORT(m_windowSize.x());
			//static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			//WriteConsoleOutput(handle, m_pWindowData, COORD{ SHORT(m_windowSize.x()), SHORT(m_windowSize.y()) }, COORD{ 0, 0 }, &m_rect);
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
private:
	static Point2D& windowSize() {
		static Point2D s_windowSize{ 40, 20 };
		return s_windowSize;
	}
	static PCHAR_INFO& pWindowData() {
		const Number area = windowSize().area();
		static PCHAR_INFO  s_pWIndowData{new CHAR_INFO[area * 2]};
		return s_pWIndowData;
	}
	static std::map<ID, std::pair<Point2D, Icon>>& objectPointInfo() {
		static std::map<ID, std::pair<Point2D, Icon>> s_objectPointInfo{};
		return s_objectPointInfo;
	}

	std::thread m_screenThread;

	Position2D m_startGamePosition; // 游戏世界坐标的左上角 
	Position2D m_endGamePosition; // 游戏世界坐标的右下角 
};

inline Screen screen;