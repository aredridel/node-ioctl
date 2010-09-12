#include <v8.h>
#include <node.h>
#include <sys/ioctl.h>

using namespace v8;

static Persistent<String> WINSZ_SYMBOL;

static Handle<Value> getwinsz (const Arguments &args) {
	if (args.Length() < 1) return Undefined();

	int fd = args[0]->ToInteger()->Value();
#ifdef TIOCGSIZE
	struct ttysize win;
#elif defined(TIOCGWINSZ)
	struct winsize win;
#endif
	HandleScope scope;
	Local<Array> winsz = Array::New(4);

#ifdef TIOCGSIZE
	if (ioctl (fd, TIOCGSIZE, &win))
		return scope.Close(Number::New(0));
	winsz->Set(Number::New(0), Integer::New(win.ts_lines));
	winsz->Set(Number::New(1), Integer::New(win.ts_cols));
	winsz->Set(Number::New(2), Integer::New(win.ts_xxx));
	winsz->Set(Number::New(3), Integer::New(win.ts_yyy));
#else
	if (ioctl (fd, TIOCGWINSZ, &win))
		return scope.Close(Integer::New(0));
	winsz->Set(Number::New(0), Integer::New(win.ws_row));
	winsz->Set(Number::New(1), Integer::New(win.ws_col));
	winsz->Set(Number::New(2), Integer::New(win.ws_xpixel));
	winsz->Set(Number::New(3), Integer::New(win.ws_ypixel));
#endif

	return scope.Close(winsz);
}

extern "C" void
init (Handle<Object> target) {
	HandleScope scope;
	NODE_SET_METHOD(target, "getwinsz", getwinsz);
}
