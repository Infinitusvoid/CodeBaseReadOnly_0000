#pragma once
#include <vector>
#include <string>

namespace MINST
{
	

	class MNISTDecoder {
	public:
		MNISTDecoder(const std::string& imageFilePath, const std::string& labelFilePath);
		bool load();
		const std::vector<std::vector<uint8_t>>& getImages() const;
		const std::vector<uint8_t>& getLabels() const;

	private:
		std::string imageFilePath;
		std::string labelFilePath;
		std::vector<std::vector<uint8_t>> images;
		std::vector<uint8_t> labels;
		int numImages;
		int numRows;
		int numCols;

		bool readImages();
		bool readLabels();
		uint32_t readUint32(std::ifstream& file);
	};

	
	

	class MNISTEncoder {
	public:
		MNISTEncoder(const std::string& imageFilePath, const std::string& labelFilePath);
		void setImageDimensions(int width, int height, int channels);
		void addImage(const std::vector<uint8_t>& image);
		void addLabel(uint8_t label);
		void setPixel(int imageIndex, int x, int y, const std::vector<uint8_t>& pixelValues);
		bool save() const;

	private:
		std::string imageFilePath;
		std::string labelFilePath;
		int imageWidth;
		int imageHeight;
		int imageChannels;
		std::vector<std::vector<uint8_t>> images;
		std::vector<uint8_t> labels;

		void writeUint32(std::ofstream& file, uint32_t value) const;
	};


	
}