#include "MINST.h"
#include <fstream>
#include <iostream>

namespace MINST
{
	// MNISTDecoder

	MNISTDecoder::MNISTDecoder(const std::string& imageFilePath, const std::string& labelFilePath)
		: imageFilePath(imageFilePath), labelFilePath(labelFilePath), numImages(0), numRows(0), numCols(0) {}

	bool MNISTDecoder::load() {
		return readImages() && readLabels();
	}

	const std::vector<std::vector<uint8_t>>& MNISTDecoder::getImages() const {
		return images;
	}

	const std::vector<uint8_t>& MNISTDecoder::getLabels() const {
		return labels;
	}

	bool MNISTDecoder::readImages() {
		std::ifstream file(imageFilePath, std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening image file: " << imageFilePath << std::endl;
			return false;
		}

		uint32_t magic = readUint32(file);
		if (magic != 2051) {
			std::cerr << "Invalid magic number in image file: " << magic << std::endl;
			return false;
		}

		numImages = readUint32(file);
		numRows = readUint32(file);
		numCols = readUint32(file);

		images.resize(numImages, std::vector<uint8_t>(numRows * numCols));
		for (int i = 0; i < numImages; ++i) {
			file.read(reinterpret_cast<char*>(images[i].data()), numRows * numCols);
		}

		return true;
	}

	bool MNISTDecoder::readLabels() {
		std::ifstream file(labelFilePath, std::ios::binary);
		if (!file.is_open()) {
			std::cerr << "Error opening label file: " << labelFilePath << std::endl;
			return false;
		}

		uint32_t magic = readUint32(file);
		if (magic != 2049) {
			std::cerr << "Invalid magic number in label file: " << magic << std::endl;
			return false;
		}

		uint32_t numLabels = readUint32(file);
		labels.resize(numLabels);
		file.read(reinterpret_cast<char*>(labels.data()), numLabels);

		return true;
	}

	uint32_t MNISTDecoder::readUint32(std::ifstream& file) {
		uint32_t value = 0;
		file.read(reinterpret_cast<char*>(&value), 4);
		return ((value & 0xFF000000) >> 24) |
			((value & 0x00FF0000) >> 8) |
			((value & 0x0000FF00) << 8) |
			((value & 0x000000FF) << 24);
	}


	// END MNISTDecoder




	// MNISTEncoder

	MNISTEncoder::MNISTEncoder(const std::string& imageFilePath, const std::string& labelFilePath)
		: imageFilePath(imageFilePath), labelFilePath(labelFilePath), imageWidth(0), imageHeight(0), imageChannels(1) {}

	void MNISTEncoder::setImageDimensions(int width, int height, int channels) {
		imageWidth = width;
		imageHeight = height;
		imageChannels = channels;
	}

	void MNISTEncoder::addImage(const std::vector<uint8_t>& image) {
		if (image.size() != imageWidth * imageHeight * imageChannels) {
			std::cerr << "Error: Image size does not match the specified dimensions." << std::endl;
			return;
		}
		images.push_back(image);
	}

	void MNISTEncoder::addLabel(uint8_t label) {
		labels.push_back(label);
	}

	bool MNISTEncoder::save() const {
		if (images.size() != labels.size()) {
			std::cerr << "Error: The number of images does not match the number of labels." << std::endl;
			return false;
		}

		std::ofstream imageFile(imageFilePath, std::ios::binary);
		if (!imageFile.is_open()) {
			std::cerr << "Error opening image file for writing: " << imageFilePath << std::endl;
			return false;
		}

		std::ofstream labelFile(labelFilePath, std::ios::binary);
		if (!labelFile.is_open()) {
			std::cerr << "Error opening label file for writing: " << labelFilePath << std::endl;
			return false;
		}

		// Write image file header
		writeUint32(imageFile, 2051);
		writeUint32(imageFile, images.size());
		writeUint32(imageFile, imageHeight);
		writeUint32(imageFile, imageWidth);
		if (imageChannels == 3) {
			writeUint32(imageFile, imageChannels);
		}

		// Write image data
		for (const auto& image : images) {
			imageFile.write(reinterpret_cast<const char*>(image.data()), image.size());
		}

		// Write label file header
		writeUint32(labelFile, 2049);
		writeUint32(labelFile, labels.size());

		// Write label data
		labelFile.write(reinterpret_cast<const char*>(labels.data()), labels.size());

		return true;
	}

	void MNISTEncoder::writeUint32(std::ofstream& file, uint32_t value) const {
		file.put((value >> 24) & 0xFF);
		file.put((value >> 16) & 0xFF);
		file.put((value >> 8) & 0xFF);
		file.put(value & 0xFF);
	}

	void MNISTEncoder::setPixel(int imageIndex, int x, int y, const std::vector<uint8_t>& pixelValues) {
		if (imageIndex < 0 || imageIndex >= images.size()) {
			std::cerr << "Error: Invalid image index." << std::endl;
			return;
		}
		if (pixelValues.size() != imageChannels) {
			std::cerr << "Error: Pixel values size does not match the number of channels." << std::endl;
			return;
		}
		if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight) {
			std::cerr << "Error: Pixel coordinates out of bounds." << std::endl;
			return;
		}

		int pixelIndex = (y * imageWidth + x) * imageChannels;
		for (int c = 0; c < imageChannels; ++c) {
			images[imageIndex][pixelIndex + c] = pixelValues[c];
		}
	}

	// END MNISTEncoder
}