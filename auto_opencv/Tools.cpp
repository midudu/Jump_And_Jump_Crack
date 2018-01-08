#include "tools.h"

void FindChess()
{
	//screenshot image
	cv::Mat origin_img = cv::imread("screenshot.png");
	cv::Mat screenshot_img = origin_img(cv::Range(370, 700), cv::Range(690, 1200));

	//chess(template) image
	cv::Mat chess_img = cv::imread("chess.png");

	//shrink image
	double zoom_factor = 0.0625;
	cv::Mat shrink_chess_image, shrink_screenshot_image;
	cv::resize(chess_img, shrink_chess_image, cv::Size(), zoom_factor, zoom_factor, CV_INTER_AREA);
	cv::resize(screenshot_img, shrink_screenshot_image, cv::Size(), zoom_factor, zoom_factor, CV_INTER_AREA);

	//result image
	int result_img_rows = shrink_screenshot_image.rows - shrink_chess_image.rows + 1;
	int result_img_cols = shrink_screenshot_image.cols - shrink_chess_image.cols + 1;

	cv::Mat result_img;
	result_img.create(result_img_rows, result_img_cols, CV_32FC1);

	//match template and normalize
	cv::matchTemplate(shrink_screenshot_image, shrink_chess_image, result_img, CV_TM_SQDIFF);
	cv::normalize(result_img, result_img, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());


	//find best match location
	double min_val, max_val;
	cv::Point min_loc, max_loc;
	cv::Point match_location;
	cv::minMaxLoc(result_img, &min_val, &max_val, &min_loc, &max_loc);

	//chess location
	match_location.x = int(min_loc.x / zoom_factor) + chess_img.cols / 2 + 690;
	match_location.y = int(min_loc.y / zoom_factor) + chess_img.rows + 370 - 10;


	std::ofstream outfile("coordinate.txt", std::ofstream::trunc);
	outfile << match_location.x << std::endl
		<< match_location.y << std::endl;
	outfile.close();
}



void FindNextBlock()
{
	//read chess coordinates from file
	int x_chess_coordinate, y_chess_coordinate;
	std::ifstream openfile("coordinate.txt");
	openfile >> x_chess_coordinate >> y_chess_coordinate;
	openfile.close();


	//screenshot image
	cv::Mat origin_img = cv::imread("screenshot.png");
	cv::Mat screenshot_img = origin_img(cv::Range(370, 700), cv::Range(690, 1200));

	x_chess_coordinate -= 690;
	y_chess_coordinate -= 370;

	//ROI of screenshot image
	cv::Mat ROI_screenshot_img;
	if (x_chess_coordinate < 255)
	{
		ROI_screenshot_img = screenshot_img(cv::Range(0, y_chess_coordinate), cv::Range(255, screenshot_img.cols));
	}
	else
	{
		ROI_screenshot_img = screenshot_img(cv::Range(0, y_chess_coordinate), cv::Range(0, 255));
	}


	cv::Mat edge_img = ROI_screenshot_img;

	cv::cvtColor(edge_img, edge_img, cv::COLOR_BGR2GRAY);

	cv::blur(edge_img, edge_img, cv::Size(3, 3));

	cv::Canny(edge_img, edge_img, 3, 9);
	//cv::imwrite("haha.png", edge_img);

	int left_top = -1, right_top = -1;
	bool break_flag = false;
	for (int i = 0; i < edge_img.rows; ++i)
	{
		uchar* data = edge_img.ptr<uchar>(i);
		for (int j = 0; j < edge_img.cols; ++j)
		{
			if (data[j] == 255)
			{
				if (left_top == -1)
					left_top = j;
				right_top = j;

				break_flag = true;
			}
		}
		if (break_flag)
		{
			std::ofstream outfile("next_block.txt", std::ofstream::trunc);
			if (x_chess_coordinate < 255)
				outfile << (left_top + right_top) / 2 + 690 + 255 << std::endl;
			else
				outfile << (left_top + right_top) / 2 + 690 << std::endl;

			outfile.close();

			return;
		}
	}

	return;
}
