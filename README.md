
# tinyvmt
a simple, small, header-only library for vmt hooking, written in c++17

# showcase
![tinyvmt in action](https://i.imgur.com/v1HJGR1.png)

# safety
only one .data section patch is applied to original process memory. 
the entire original vtable is copied over to a new instance, where hooked functions are pointer-swapped.

# usage
drag and drop the header file into your project, include it and you're almost set!
```cpp
// include tinyvmt into your project
#include <tinyvmt.hh>

// one class instance should be created per object
tinyvmt::c_vmt instance;

// hook a function
instance.create( pointer_to_object_with_vtable );
instance.hook( index_of_hooked_function, reinterpret_cast<void*>(&hooked_fn) );

// unhook a function
instance.unhook( index_of_hooked_function );

// restore original vtable pointer
instance.restore( );

// inside the hooked function
void __thiscall hooked_fn( void* thisptr ) {
  // your code goes here...

  // call original function
  instance.get_old_function< decltype ( &hooked_fn ) > ( index_of_hooked_function ) ( thisptr );
}
```

# extra
if you think i missed anything or have any questions, feel free to open an issue. 
if you'd like to contribute, pull requests are always welcome.
