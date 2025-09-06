#include "Tickable.h"
#include "Texture.h"

namespace Beckett
{
	class NineSlicer : public Tickable
	{
	private:
		std::shared_ptr<Texture> texture;
		int left, top, width, height;

	public:
		NineSlicer(const std::string& texture, int left, int top, int width, int height);
		void Draw(float dt);
	};
}
