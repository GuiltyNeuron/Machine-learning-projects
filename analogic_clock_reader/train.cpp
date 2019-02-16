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
	Mat Gray, Transposed, Reshaped, Feautures, squared;

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

int main(int argc, char *argv[])
{
	char file_name[100];

	// Variables declaration
	Mat trainingDataMat, transposed, reshaped, image, gray_image, thresh, feautures;
	Mat test, testGray, testThresh, testTransposed, testReshaped, testFeautures, squared;
	int label = 0;
	int i = 0;
	const int samples = 701;
	int labels[samples];


	std::cout << "***********************************************************" << endl;
	std::cout << "\nImporting training features and their labels" << endl;

	// Dataset preparing
	//*************************************************************************************************************************************
	for (auto& dirEntry : recursive_directory_iterator(argv[1]))
	{
		// File path
		fs::path filePath(dirEntry);

		// Case of directory
		if (!filePath.has_extension())
		{
			// BARCODE label
			if (filePath.filename() == "1D") { label = 1; }
			else if (filePath.filename() == "2D") { label = 2; }
			else { label = 0; }

			//label = filePath.filename() == "barcode";
			std::cout << "Loading features for " << filePath.filename() << " label " << label << endl;

			// In Diractory case ignore the rest of the code
			continue;
		}


		// read the image
		image = cv::imread(filePath.string(), CV_LOAD_IMAGE_COLOR);

		// Apply preprocess to the image
		transposed = process(image);

		// Append the image to Opencv MAT
		trainingDataMat.push_back(transposed);

		// Append label to labels list
		labels[i] = label;
		i = i + 1;
	}

	// Convert Labels list to OpenCV MAT with the appropriate variables type
	Mat labelsMat(samples, 1, CV_32SC1, labels);



	// Change the type of features
	trainingDataMat.convertTo(feautures, CV_32F);

	//feautures = process(image);
	std::cout << "Features Matrix size : " << feautures.size << endl;
	std::cout << "Labels Matrix size : " << labelsMat.size << endl;






	// Support Vector Machine Training
	//*************************************************************************************************************************************
	std::cout << "***********************************************************" << endl;
	std::cout << "\nClassifier training" << endl;

	// Set Support vector machine parameters
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-5));

	// Feed the classifier with training features and labels
	svm->train(feautures, ROW_SAMPLE, labelsMat);
	std::cout << "Training successefuly completed" << endl;
	svm->save("../../model/SVM.xml");
	std::cout << "***********************************************************" << endl;

  return 0;
}
