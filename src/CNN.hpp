#ifndef _CNN_HPP_
#define _CNN_HPP_

namespace ANN {

#define width_image_input_CNN		32 //归一化图像宽
#define height_image_input_CNN		32 //归一化图像高
#define width_image_C1_CNN		28
#define height_image_C1_CNN		28
#define width_image_S2_CNN		14
#define height_image_S2_CNN		14
#define width_image_C3_CNN		10
#define height_image_C3_CNN		10
#define width_image_S4_CNN		5
#define height_image_S4_CNN		5
#define width_image_C5_CNN		1
#define height_image_C5_CNN		1
#define width_image_output_CNN		1
#define height_image_output_CNN		1

#define width_kernel_conv_CNN		5 //卷积核大小
#define height_kernel_conv_CNN		5
#define width_kernel_pooling_CNN	2
#define height_kernel_pooling_CNN	2
#define size_pooling_CNN		2

#define num_map_input_CNN		1 //输入层map个数
#define num_map_C1_CNN			6 //C1层map个数
#define num_map_S2_CNN			6 //S2层map个数
#define num_map_C3_CNN			16 //C3层map个数
#define num_map_S4_CNN			16 //S4层map个数
#define num_map_C5_CNN			120 //C5层map个数
#define num_map_output_CNN		10 //输出层map个数

#define num_patterns_train_CNN		60000 //训练模式对数(总数)
#define num_patterns_test_CNN		10000 //测试模式对数(总数)
#define num_epochs_CNN			100 //最大迭代次数
#define accuracy_rate_CNN		0.97 //要求达到的准确率
#define learning_rate_CNN		0.01 //学习率
#define eps_CNN				1e-8

#define len_weight_C1_CNN		150 //C1层权值数，5*5*6=150
#define len_bias_C1_CNN			6 //C1层阈值数，6
#define len_weight_S2_CNN		6 //S2层权值数,1*6=6
#define len_bias_S2_CNN			6 //S2层阈值数,6
#define len_weight_C3_CNN		2400 //C3层权值数，5*5*6*16
#define len_bias_C3_CNN			16 //C3层阈值数,16
#define len_weight_S4_CNN		16 //S4层权值数，1*16=16
#define len_bias_S4_CNN			16 //S4层阈值数，16
#define len_weight_C5_CNN		48000 //C5层权值数，5*5*16*120=48000
#define len_bias_C5_CNN			120 //C5层阈值数，120
#define len_weight_output_CNN		1200 //输出层权值数，120*10=1200
#define len_bias_output_CNN		10 //输出层阈值数，10

#define num_neuron_input_CNN		1024 //输入层神经元数，32*32=1024
#define num_neuron_C1_CNN		4704 //C1层神经元数，28*28*6=4704
#define num_neuron_S2_CNN		1176 //S2层神经元数，14*14*6=1176
#define num_neuron_C3_CNN		1600 //C3层神经元数，10*10*16=1600
#define num_neuron_S4_CNN		400 //S4层神经元数，5*5*16=400
#define num_neuron_C5_CNN		120 //C5层神经元数，1*120=120
#define num_neuron_output_CNN		10 //输出层神经元数，1*10=10

class CNN {
public:
	CNN();
	~CNN();

	void init(); //初始化，分配空间
	bool train(); //训练
	int predict(const unsigned char* data, int width, int height); //预测
	bool readModelFile(const char* name); //读取已训练好的BP model

protected:
	typedef std::vector<std::pair<int, int> > wi_connections;
	typedef std::vector<std::pair<int, int> > wo_connections;
	typedef std::vector<std::pair<int, int> > io_connections;

	void release(); //释放申请的空间
	bool saveModelFile(const char* name); //将训练好的model保存起来，包括各层的节点数，权值和阈值
	bool initWeightThreshold(); //初始化，产生[-1, 1]之间的随机小数
	bool getSrcData(); //读取MNIST数据
	float test(); //训练完一次计算一次准确率
	float activation_function_tanh(float x); //激活函数:tanh
	float activation_function_tanh_derivative(float x); //激活函数tanh的导数
	float activation_function_identity(float x);
	float activation_function_identity_derivative(float x);
	float loss_function_mse(float y, float t); //损失函数:mean squared error
	float loss_function_mse_derivative(float y, float t);
	void loss_function_gradient(const float* y, const float* t, float* dst, int len);
	float dot_product(const float* s1, const float* s2, int len); //点乘
	bool muladd(const float* src, float c, int len, float* dst); //dst[i] += c * src[i]
	void init_variable(float* val, float c, int len);
	bool uniform_rand(float* src, int len, float min, float max);
	float uniform_rand(float min, float max);
	int get_index(int x, int y, int channel, int width, int height, int depth);
	void calc_out2wi(int width_in, int height_in, int width_out, int height_out, int depth_out, std::vector<wi_connections>& out2wi);
	void calc_out2bias(int width, int height, int depth, std::vector<int>& out2bias);
	void calc_in2wo(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<wo_connections>& in2wo);
	void calc_weight2io(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<io_connections>& weight2io);
	void calc_bias2out(int width_in, int height_in, int width_out, int height_out, int depth_in, int depth_out, std::vector<std::vector<int> >& bias2out);

	bool Forward_C1(); //前向传播
	bool Forward_S2();
	bool Forward_C3();
	bool Forward_S4();
	bool Forward_C5();
	bool Forward_output();
	bool Backward_output();
	bool Backward_C5(); //反向传播
	bool Backward_S4();
	bool Backward_C3();
	bool Backward_S2();
	bool Backward_C1();
	bool Backward_input();
	bool UpdateWeights(); //更新权值、阈值
	void update_weights_bias(const float* delta, float* weight, int len);

private:
	float* data_input_train; //原始标准输入数据，训练,范围：[-1, 1]
	float* data_output_train; //原始标准期望结果，训练,范围：[-0.9, 0.9]
	float* data_input_test; //原始标准输入数据，测试,范围：[-1, 1]
	float* data_output_test; //原始标准期望结果，测试,范围：[-0.9, 0.9]
	float* data_single_image;
	float* data_single_label;

	float weight_C1[len_weight_C1_CNN];
	float bias_C1[len_bias_C1_CNN];
	float weight_S2[len_weight_S2_CNN];
	float bias_S2[len_bias_S2_CNN];
	float weight_C3[len_weight_C3_CNN];
	float bias_C3[len_bias_C3_CNN];
	float weight_S4[len_weight_S4_CNN];
	float bias_S4[len_bias_S4_CNN];
	float weight_C5[len_weight_C5_CNN];
	float bias_C5[len_bias_C5_CNN];
	float weight_output[len_weight_output_CNN];
	float bias_output[len_bias_output_CNN];

	float neuron_input[num_neuron_input_CNN]; //data_single_image
	float neuron_C1[num_neuron_C1_CNN];
	float neuron_S2[num_neuron_S2_CNN];
	float neuron_C3[num_neuron_C3_CNN];
	float neuron_S4[num_neuron_S4_CNN];
	float neuron_C5[num_neuron_C5_CNN];
	float neuron_output[num_neuron_output_CNN];

	float delta_neuron_output[num_neuron_output_CNN]; //神经元误差
	float delta_neuron_C5[num_neuron_C5_CNN];
	float delta_neuron_S4[num_neuron_S4_CNN];
	float delta_neuron_C3[num_neuron_C3_CNN];
	float delta_neuron_S2[num_neuron_S2_CNN];
	float delta_neuron_C1[num_neuron_C1_CNN];
	float delta_neuron_input[num_neuron_input_CNN];

	float delta_weight_C1[len_weight_C1_CNN]; //权值、阈值误差
	float delta_bias_C1[len_bias_C1_CNN];
	float delta_weight_S2[len_weight_S2_CNN];
	float delta_bias_S2[len_bias_S2_CNN];
	float delta_weight_C3[len_weight_C3_CNN];
	float delta_bias_C3[len_bias_C3_CNN];
	float delta_weight_S4[len_weight_S4_CNN];
	float delta_bias_S4[len_bias_S4_CNN];
	float delta_weight_C5[len_weight_C5_CNN];
	float delta_bias_C5[len_bias_C5_CNN];
	float delta_weight_output[len_weight_output_CNN];
	float delta_bias_output[len_bias_output_CNN];

	std::vector<wi_connections> out2wi_S2; // out_id -> [(weight_id, in_id)]
	std::vector<int> out2bias_S2;
	std::vector<wi_connections> out2wi_S4;
	std::vector<int> out2bias_S4;
	std::vector<wo_connections> in2wo_C3; // in_id -> [(weight_id, out_id)]
	std::vector<io_connections> weight2io_C3; // weight_id -> [(in_id, out_id)]
	std::vector<std::vector<int> > bias2out_C3;
	std::vector<wo_connections> in2wo_C1;
	std::vector<io_connections> weight2io_C1;
	std::vector<std::vector<int> > bias2out_C1;
};

}

#endif //_CNN_HPP_
