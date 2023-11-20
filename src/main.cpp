#include <LCompositor.h>
#include <LLog.h>
#include "jorge.hpp"

using namespace Louvre;

int main()
{
	JCompositor compositor;

	if (!compositor.start()) {
		LLog::fatal("[louvre-example] Failed to start compositor.");
		return 1;
	}

	while (compositor.state() != LCompositor::Uninitialized)
		compositor.processLoop(-1);

	return 0;
}
