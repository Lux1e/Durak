#pragma once
#include "../protocol/PacketType.h"
#include <vector>
#include <string>


class Packet
{
public:
	Packet(PacketType type) : _type(type), _readPosition(0) {}

	Packet(const std::vector<uint8_t>&& buffer)
	{
		_type = static_cast<PacketType>(buffer[4]);
		_data.assign(buffer.begin() + 5, buffer.end());
		_readPosition = 0;
	}


	template <std::integral T>
		requires (!std::is_same_v<T, bool> && !std::is_same_v<T, char>)
	void write(T value)
	{
		using U = std::make_unsigned_t<T>;
		U uValue = static_cast<U>(value);

		for (size_t i = sizeof(uValue); i-- > 0; )
			_data.push_back(static_cast<uint8_t>((uValue >> (i * 8)) & 0xff));
	}

	template<typename E>
		requires (std::is_enum_v<E>)
	void write(E value)
	{
		using U = std::underlying_type_t<E>;
		write(static_cast<U>(value));
	}

	void write(bool value)
	{
		_data.push_back(static_cast<uint8_t>(value));
	}

	void write(const std::string& string)
	{
		write<uint32_t>(string.size());
		_data.insert(_data.end(), string.begin(), string.end());
	}


	std::vector<uint8_t> serialize() const
	{
		std::vector<uint8_t> buffer;
		uint32_t bodyLen = 1 + (uint32_t)_data.size();

		for (size_t i = sizeof(bodyLen); i-- > 0; )
		{
			buffer.push_back(bodyLen >> (i * 8) & 0xff);
		}

		buffer.push_back((uint8_t)_type);

		buffer.insert(buffer.end(), _data.begin(), _data.end());

		return buffer;
	}


	template <std::integral T>
		requires (!std::is_same_v<T, bool> && !std::is_same_v<T, char>)
	T read()
	{
		using U = std::make_unsigned_t<T>;
		U value = 0;

		for (size_t i = 0; i < sizeof(value); ++i)
		{
			size_t shift = (sizeof(value) - 1 - i) * 8;
			value |= (static_cast<U>(_data[_readPosition + i]) << shift);
		}

		_readPosition += sizeof(U);

		return static_cast<T>(value);
	}

	template <typename E>
		requires std::is_enum_v<E>
	E read()
	{
		using U = std::underlying_type_t<E>;
		return static_cast<E>(read<U>());
	}

	template<typename T>
		requires std::is_same_v<T, bool>
	T read()
	{
		return static_cast<bool>(_data[_readPosition++]);
	}

	template<typename T>
		requires std::is_same_v<T, std::string>
	T read()
	{
		int32_t len = read<uint32_t>();
		std::string string(_data.begin() + _readPosition, _data.begin() + _readPosition + len);

		_readPosition += len;

		return string;
	}


	PacketType getType() const
	{
		return _type;
	}

private:
	PacketType _type;
	std::vector<uint8_t> _data;
	size_t _readPosition;
};