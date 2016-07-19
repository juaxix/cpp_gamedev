/// Sdl.h
/// Header of the class Sdl

#pragma once
namespace xixEngine {
	///<summary>Initializes SDL with specified flags</summary>
	class Sdl {
	public:
		Sdl(int flags);
		~Sdl();
	private:
		///Default copy constructor: avoid copies in the C++98 way (private)
		Sdl(const Sdl&); 

		///Avoid assigments of this class in the C++11 way (=default, =delete)
		Sdl& operator=(const Sdl&) = delete;
	};
}
