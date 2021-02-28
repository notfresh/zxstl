#ifndef _SUFFIX_ARRAY_H_
#define _SUFFIX_ARRAY_H_

#include <vector>

namespace TinySTL{

	class suffix_array{
	public:
		using array_type = std::vector < int > ;
	private:
		array_type _suffix_array;
		array_type _height_array;
		array_type _rank_array;
	public:
		template<class InputIterator>
		//arr - Դ����
		//len - Դ���鳤��
		//max_len - max_len�����ַ���arr���ַ���ȡֵ��Χ���ǻ��������һ��������ԭ���ж�����ĸ���Կ���ֱ��ȡ256
		suffix_array(const InputIterator arr, size_t len, size_t max_len = 256){
			if (max_len > 256)
				throw std::exception("out of the range");
			calSuffix(arr, len, max_len);
			calRank();
			calHeight(arr, len);
		}

		array_type suffixArray()const{ return _suffix_array; }
		array_type heightArray()const{ return _height_array; }
		array_type rankArray()const{ return _rank_array; }
	private:
		template<class InputIteraotr>
		bool cmp(const InputIteraotr arr, size_t a, size_t b, size_t l);
		void calRank(){
			_rank_array.resize(_suffix_array.size());
			for (auto i = 0; i != _suffix_array.size(); ++i){
				_rank_array[_suffix_array[i]] = i;
			}
		}
		template<class InputIterator>
		void calSuffix(const InputIterator arr, size_t len, size_t max_len);
		template<class InputIteraotr>
		void calHeight(const InputIteraotr arr, size_t len);
	};

	template<class InputIteraotr>
	bool suffix_array::cmp(const InputIteraotr arr, size_t a, size_t b, size_t l){
		return arr[a] == arr[b] && arr[a + l] == arr[b + l];
	}
	template<class InputIteraotr>
	void suffix_array::calHeight(const InputIteraotr arr, size_t len){
		_height_array.resize(_suffix_array.size() - 1);
		for (auto i = 0; i != _suffix_array.size() - 1; ++i){
			auto n = 0;
			for (auto j = _suffix_array[i], k = _suffix_array[i + 1];
				arr[j] == arr[k] && (arr + j) != (arr + len) && (arr + k) != (arr + len);
				++j, ++k)
				++n;
			_height_array[i] = n;
		}
	}
	template<class InputIterator>
	void suffix_array::calSuffix(const InputIterator arr, size_t len, size_t max_len){
		//�����������������ʵ�ֵı����㷨
		//�㷨ʱ�临�Ӷ� = O(nlg(n))
		_suffix_array.resize(len);
		int wa[256], wb[256], wv[256], ws[256];
		int i, j, p, *x = wa, *y = wb, *t;

		//�������д����ǰѸ����ַ���Ҳ������Ϊ1���ַ��������л�������
		for (i = 0; i < max_len; i++) ws[i] = 0;
		//x[]���汾���Ǳ��������׺��rankֵ�ģ��������ﲢû��ȥ�洢rankֵ����Ϊ����ֻ���漰x[]�ıȽϹ����������һ�����Բ��ô洢��ʵ��rankֵ���ܹ���ӳ��ԵĴ�С���ɡ�
		for (i = 0; i < len; i++) ws[x[i] = arr[i]]++;
		for (i = 1; i < max_len; i++) ws[i] += ws[i - 1];
		//i֮���Դ�len-1��ʼѭ������Ϊ�˱�֤�ڵ��ַ���������ȵ��ַ���ʱ��Ĭ�Ͽ�ǰ���ַ�����СһЩ��
		for (i = len - 1; i >= 0; i--) _suffix_array[--ws[x[i]]] = i;

		//�������ѭ����p����rankֵ���õ��ַ��������������p�ﵽlen����ô�����ַ����Ĵ�С��ϵ���Ѿ������ˡ�
		//j����ǰ���ϲ����ַ����ĳ��ȣ�ÿ�ν���������Ϊj���ַ����ϲ���һ������Ϊ2*j���ַ�������Ȼ��������ַ���ĩβ��������ֵӦ�����ۣ���˼����һ���ġ�
		//max_lenͬ��������������Ԫ�ص�ȡֵ��Χ
		for (j = 1, p = 1; p < len; j *= 2, max_len = p){
			//�������д���ʵ���˶Եڶ��ؼ��ֵ�����
			for (p = 0, i = len - j; i < len; i++) y[p++] = i;
			for (i = 0; i < len; i++) if (_suffix_array[i] >= j) y[p++] = _suffix_array[i] - j;
			//�ڶ��ؼ��ֻ���������ɺ�y[]���ŵ��ǰ��ڶ��ؼ���������ַ����±�
			//�����൱����ȡ��ÿ���ַ����ĵ�һ�ؼ��֣�ǰ��˵����x[]�Ǳ���rankֵ�ģ�Ҳ�����ַ����ĵ�һ�ؼ��֣����ŵ�wv[]�����Ƿ�������ʹ��
			//�������д����ǰ���һ�ؼ��ֽ��еĻ�������
			for (i = 0; i < len; i++) wv[i] = x[y[i]];
			for (i = 0; i < max_len; i++) ws[i] = 0;
			for (i = 0; i < len; i++) ws[wv[i]]++;
			for (i = 1; i < max_len; i++) ws[i] += ws[i - 1];
			for (i = len - 1; i >= 0; i--) _suffix_array[--ws[wv[i]]] = y[i];
			//�������о��Ǽ���ϲ�֮���rankֵ�ˣ����ϲ�֮���rankֵӦ�ô���x[]���棬�����Ǽ����ʱ���ֱ����õ���һ���rankֵ��Ҳ��������x[]����ŵĶ���������Ҽ�Ҫ��x[]�����ã���Ҫ��x[]����ţ���ô�죿
			//��Ȼ���Ȱ�x[]�Ķ����ŵ�����һ���������棬ʡ�����ˡ���������ý���ָ��ķ�ʽ����Чʵ���˽�x[]�Ķ��������ơ�����y[]�С�
			for (t = x, x = y, y = t, p = 1, x[_suffix_array[0]] = 0, i = 1; i < len; i++)
				x[_suffix_array[i]] = cmp(y, _suffix_array[i - 1], _suffix_array[i], j) ? p - 1 : p++;
		}
	}
}

#endif