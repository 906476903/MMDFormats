//          Copyright oguna 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>

namespace vmd
{
	/// �{�[���t���[��
	class VmdBoneFrame
	{
	public:
		/// �{�[����
		std::string name;
		/// �t���[���ԍ�
		int frame;
		/// �ʒu
		float position[3];
		/// ��]
		float orientation[4];
		/// ��ԋȐ�
		char interpolation[4][4][4];

		void Read(std::istream* stream)
		{
			char buffer[15];
			stream->read((char*) buffer, sizeof(char)*15);
			name = std::string(buffer);
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) position, sizeof(float)*3);
			stream->read((char*) orientation, sizeof(float)*4);
			stream->read((char*) interpolation, sizeof(char) * 4 * 4 * 4);
		}
	};

	/// �\��t���[��
	class VmdFaceFrame
	{
	public:
		/// �\�
		std::string face_name;
		/// �\��̏d��
		float weight;
		/// �t���[���ԍ�
		uint32_t frame;

		void Read(std::istream* stream)
		{
			char buffer[15];
			stream->read((char*) &buffer, sizeof(char) * 15);
			face_name = std::string(buffer);
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) &weight, sizeof(float));
		}
	};

	/// �J�����t���[��
	class VmdCameraFrame
	{
	public:
		/// �t���[���ԍ�
		int frame;
		/// ����
		float distance;
		/// �ʒu
		float position[3];
		/// ��]
		float orientation[3];
		/// ��ԋȐ�
		char interpolation[6][4];
		/// ����p
		float angle;
		/// �s���f�[�^
		char unknown[3];

		void Read(std::istream *stream)
		{
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) &distance, sizeof(float));
			stream->read((char*) position, sizeof(float) * 3);
			stream->read((char*) orientation, sizeof(float) * 3);
			stream->read((char*) interpolation, sizeof(char) * 24);
			stream->read((char*) &angle, sizeof(float));
			stream->read((char*) unknown, sizeof(char) * 3);
		}
	};

	/// ���C�g�t���[��
	class VmdLightFrame
	{
	public:
		/// �t���[���ԍ�
		int frame;
		/// �F
		float color[3];
		/// �ʒu
		float position[3];

		void Read(std::istream* stream)
		{
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) color, sizeof(float) * 3);
			stream->read((char*) position, sizeof(float) * 3);
		}
	};

	/// IK�̗L������
	class VmdIkEnable
	{
	public:
		std::string ik_name;
		bool enable;
	};

	/// IK�t���[��
	class VmdIkFrame
	{
	public:
		int frame;
		bool display;
		std::vector<VmdIkEnable> ik_enable;

		void Read(std::istream *stream)
		{
			char buffer[20];
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) &display, sizeof(uint8_t));
			int ik_count;
			stream->read((char*) &ik_count, sizeof(int));
			ik_enable.resize(ik_count);
			for (int i = 0; i < ik_count; i++)
			{
				stream->read(buffer, 20);
				ik_enable[i].ik_name = std::string(buffer);
				stream->read((char*) &ik_enable[i].enable, sizeof(uint8_t));
			}
		}
	};

	/// VMD���[�V����
	class VmdMotion
	{
	public:
		/// ���f����
		std::string model_name;
		/// �o�[�W����
		int version;
		/// �{�[���t���[��
		std::vector<VmdBoneFrame> bone_frames;
		/// �\��t���[��
		std::vector<VmdFaceFrame> face_frames;
		/// �J�����t���[��
		std::vector<VmdCameraFrame> camera_frames;
		/// ���C�g�t���[��
		std::vector<VmdLightFrame> light_frames;
		/// IK�t���[��
		std::vector<VmdIkFrame> ik_frames;

		static std::unique_ptr<VmdMotion> LoadFromFile(char const *filename)
		{
			std::ifstream stream(filename, std::ios::binary);
			auto result = LoadFromStream(&stream);
			stream.close();
			return result;
		}

		static std::unique_ptr<VmdMotion> LoadFromStream(std::ifstream *stream)
		{

			char buffer[30];
			auto result = std::make_unique<VmdMotion>();

			// magic and version
			stream->read((char*) buffer, 30);
			if (strncmp(buffer, "Vocaloid Motion Data", 20))
			{
				std::cerr << "invalid vmd file." << std::endl;
				return nullptr;
			}
			result->version = std::atoi(buffer + 20);

			// name
			stream->read(buffer, 20);
			result->model_name = std::string(buffer);

			// bone frames
			int bone_frame_num;
			stream->read((char*) &bone_frame_num, sizeof(int));
			result->bone_frames.resize(bone_frame_num);
			for (uint16_t i = 0; i < bone_frame_num; i++)
			{
				result->bone_frames[i].Read(stream);
			}

			// face frames
			int face_frame_num;
			stream->read((char*) &face_frame_num, sizeof(int));
			result->face_frames.resize(face_frame_num);
			for (int i = 0; i < face_frame_num; i++)
			{
				result->face_frames[i].Read(stream);
			}

			// camera frames
			int camera_frame_num;
			stream->read((char*) &camera_frame_num, sizeof(int));
			result->camera_frames.resize(camera_frame_num);
			for (int i = 0; i < camera_frame_num; i++)
			{
				result->camera_frames[i].Read(stream);
			}

			// light frames
			int light_frame_num;
			stream->read((char*) &light_frame_num, sizeof(int));
			result->light_frames.resize(light_frame_num);
			for (int i = 0; i < light_frame_num; i++)
			{
				result->light_frames[i].Read(stream);
			}

			// unknown2
			stream->read(buffer, 4);

			// ik frames
			if (!stream->eof())
			{
				int ik_num;
				stream->read((char*) &ik_num, sizeof(int));
				result->ik_frames.resize(ik_num);
				for (int i = 0; i < ik_num; i++)
				{
					result->ik_frames[i].Read(stream);
				}
			}

			if (!stream->eof())
			{
				std::cerr << "vmd stream has unknown data." << std::endl;
			}

			return result;
		}
	};
}