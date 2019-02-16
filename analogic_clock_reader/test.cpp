#include "train.h"


using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;
using std::experimental::filesystem::recursive_directory_iterator;
using namespace cv::ml;


Mat square(Mat img, int size)
{
	Mat squared, x1, x2, resized;
	int width = img.cols;
	int height = img.rows;

	// Difference between height and weight
	int difference = abs(height - width);

	// In case height superior then weight
	if (height > width)
	{
		// In case the difference is pair
		if (difference % 2 == 0)
		{
			// Build two matrix with same size to concatenate them with img and having the same height and weight
			x1 = Mat::ones(height, int(difference / 2), CV_8U) * 255;
			x2 = Mat::ones(height, int(difference / 2), CV_8U) * 255;


		}

		// In case the difference is unpair
		else
		{
			// Build two matrix one with pair size and the other with unpair size to concatenate them with img and having the same height and weight
			x1 = Mat::ones(height, int(difference / 2), CV_8U) * 255;
			x2 = Mat::ones(height, int((difference / 2) + 1), CV_8U) * 255;
		}

		// Horizontal concatenation of matrix x1 and x2 with the input
		hconcat(x1, img, squared);
		hconcat(squared, x2, squared);

		// Resize the result
		resize(squared, resized, Size(size, size));

		return resized;
	}

	// In case weight superior then height
	else if (width > height)
	{
		if (difference % 2 == 0)
		{
			// Build two matrix with same size to concatenate them with img and having the same height and weight
			x1 = Mat::ones(int(difference / 2), width, CV_8U) * 255;
			x2 = Mat::ones(int(difference / 2), width, CV_8U) * 255;


		}
		else
		{
			// Build two matrix one with pair size and the other with unpair size to concatenate them with img and having the same height and weight
			x1 = Mat::ones(int(difference / 2), width, CV_8U) * 255;
			x2 = Mat::ones(int((difference / 2) + 1), width, CV_8U) * 255;
		}

		// Vertical concatenation of matrix x1 and x2 with the input
		vconcat(x1, img, squared);
		vconcat(squared, x2, squared);

		// Resize the result
		resize(squared, resized, Size(size, size));

		return resized;

	}

	// In case height and width are equal
	else if (height == width)
	{
		// Resize the result
		resize(img, resized, Size(size, size));
		return resized;
	}

	else return img;

}


Mat process(Mat img)
{
	Mat Gray, Transposed, Reshaped, Feautures, squared, croped;

	// Convert RGB image to grayscaled image
	cv::cvtColor(img, Gray, CV_BGR2GRAY);

	// Normalize the image
	squared = square(Gray, 100);

	if (squared.isContinuous())
	{
		// Convert image to 1 dimension vector
		Reshaped = squared.reshape(1, squared.rows * squared.cols);

		// Transpose from vertical vector to horizontal
		Transposed = Reshaped.t();

	}

	return Transposed;
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int main(int argc, char *argv[])

{

	std::cout << "===========================================================" << endl;

	// Variables declaration
	Mat trainingDataMat, transposed, reshaped,image, gray_image, thresh, feautures;
	Mat test, testGray, testThresh, testTransposed, testReshaped, testFeautures,squared;
	int label = 0;
	int i = 0;
	const int samples = 2000;
	int labels[samples];

	std::cout << "===========================================================" << endl;


	// Dataset preparing
	//***********************************************************************************
	for (auto& dirEntry : recursive_directory_iterator(argv[1]))
	{
		// File path
		fs::path filePath(dirEntry);

		// Case of directory
		if (!filePath.has_extension())
		{
			continue;
		}


		// read the image
		image = cv::imread(filePath.string(), CV_LOAD_IMAGE_COLOR);

		// Apply preprocess to the image
		transposed = process(image);

		// Append the image to Opencv MAT
		trainingDataMat.push_back(transposed);

	}

	// Change the type of features
	trainingDataMat.convertTo(feautures, CV_32F);

	// Load and run the classifier
	Ptr<SVM> svm = SVM::load(argv[2]);

	int one=0, zero=0,two=0 ;
	clock_t evalStart = clock();
	for (int i = 0; i < trainingDataMat.rows; i++) {
		Mat sample;
		trainingDataMat.row(i).convertTo(sample, CV_32F);



		int pred = svm->predict(sample);


	}

	cout << "===========================================================" << endl;


	return 0;
}
