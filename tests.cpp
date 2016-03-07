#include "imageRecognition.h"

int main(int argc, char** argv)
{
	ImageRecognition* ir = new ImageRecognition();

	//ir->getPixelsBetweenThreads("inputImages/tpi_just_thread.jpg", "outputImages/tpi_just_thread.jpg");
	//ir->detectLines("inputImages/tpi1.jpg", "outputImages/tpi1.jpg");
	//ir->detectLines("inputImages/tpi2.jpg", "outputImages/tpi2.jpg");
	//ir->getThreadPeakDiameter("inputImages/tpi_just_thread.jpg", "outputImages/tpi_just_thread.jpg");
	
	ir->detectCircles("inputImages/purple-bg1.jpg", "outputImages/purple-bg1.jpg");
	ir->detectCircles("inputImages/purple-bg2.jpg", "outputImages/purple-bg2.jpg");
	ir->detectCircles("inputImages/purple-bg3.jpg", "outputImages/purple-bg3.jpg");
	ir->detectCircles("inputImages/purple-bg4.jpg", "outputImages/purple-bg4.jpg");
	ir->detectCircles("inputImages/purple-bg5.jpg", "outputImages/purple-bg5.jpg");
	ir->detectCircles("inputImages/purple-bg6.jpg", "outputImages/purple-bg6.jpg");
	ir->detectCircles("inputImages/purple-bg7.jpg", "outputImages/purple-bg7.jpg");

	ir->detectCircles("inputImages/green-bg1.jpg", "outputImages/green-bg1.jpg");
	ir->detectCircles("inputImages/a-carpet.jpg", "outputImages/a-carpet.jpg");
	ir->detectCircles("inputImages/1242060-easy.jpg", "outputImages/1242060-easy.jpg");
	ir->detectCircles("inputImages/1242060_hard.jpg", "outputImages/1242060_hard.jpg");
	ir->detectCircles("inputImages/1242060-carpet.jpg", "outputImages/1242060-carpet.jpg");
	ir->detectCircles("inputImages/1242060-carpet2.jpg", "outputImages/1242060-carpet2.jpg");
	ir->detectCircles("inputImages/1498476-easy.jpg", "outputImages/1498476-easy.jpg");
	ir->detectCircles("inputImages/two-white.jpg", "outputImages/two-white.jpg");
	
}