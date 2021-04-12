#pragma once

class Runnable
{
public:
	virtual ~Runnable() { }
	virtual void run() const = 0;
};

