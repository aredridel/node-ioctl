#include <v8.h>
#include <sys/ioctl.h>

using namespace v8;

static Persistent<String> WINSZ_SYMBOL;

class Ioctl : public Object {
	public:
	static void Initialize (Handle<Object> target) {
		HandleScope scope;
		Local<FunctionTemplate> t = FunctionTemplate::New(New);

		t->Inherit(Object::constructor_template);
		t->InstanceTemplate()->SetInternalFieldCount(1);

		t->PrototypeTemplate()->SetAccessor(WINSZ_SYMBOL, WinSZGetter);

	}

	static Handle<Value> WinSZGetter (Local<String> property, const AccessorInfo& info) {
		int fd = 1;
#ifdef TIOCGSIZE
		struct ttysize win;
#elif defined(TIOCGWINSZ)
		struct winsize win;
#endif
		HandleScope scope;
		Local<Array> winsz = Array::New(4);

#ifdef TIOCGSIZE
		if (ioctl (fd, TIOCGSIZE, &win))
			return scope.Close(Integer::New(0));
	        winsz->Set(Integer::New(win.ts_lines), 0);
	        winsz->Set(Integer::New(win.ts_cols), 1);
	        winsz->Set(Integer::New(win.ts_xxx), 2);
	        winsz->Set(Integer::New(win.ts_yyy), 3);
#else
		if (ioctl (fd, TIOCGWINSZ, &win))
			return scope.Close(Integer::New(0));
		winsz->Set(Integer::New(win.ws_row), 0);
		winsz->Set(Integer::New(win.ws_col), 1);
		winsz->Set(Integer::New(win.ws_xpixel), 2);
		winsz->Set(Integer::New(win.ws_ypixel), 3);
#endif

		return scope.Close(winsz);
	}


}

extern "C" void
init (Handle<Object> target) 
{
  HandleScope scope;
  Ioctl.Initialize(target);
  target->Set(String::New("hello"), String::New("World"));
}
