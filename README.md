# Elevators
![image](https://github.com/liuxier-404/Elevators/blob/master/QQ%E6%88%AA%E5%9B%BE20191224131803.jpg)
大二数据结构-大作业1电梯够吗？
一．题目要求：
根据所给的写字楼人流情况模拟电梯的运行，给出每一天的来访者平均满意率和
增加/减少几部电梯，能够提高/保持平均满意率大于70%两个统计结果
二．题目假设：
电梯按照最先抵达优先响应；
每个人到达1-22层的概率是相等的；
电梯运行每一层：停留时间2分钟，不停留：0.5分钟
进入写字楼的人都是从一楼进入，离开写字楼的人目的地都是一楼
不存在同一楼层出发到达同一楼层的现象
三．模拟结果：
因为数据是随机生成的，所以每一次的满意率和平均等待时间都有差异。为了取得一个误差较小的值，运行十次程序，取平均值。
第一次：
平均等待时间：266.862727分钟
满意率：2.545455%

第二次：
平均等待时间：415.263409分钟
满意率：3.015152%

第三次：
平均等待时间：265.960682分钟
满意率：2.560606%

第四次：
平均等待时间：346.523485分钟
满意率：2.712121%

第五次：
平均等待时间：290.366212分钟
满意率：2.651515%

第六次：
平均等待时间：333.660606分钟
满意率：2.393939%

第七次：
平均等待时间：298.508333分钟
满意率：3.015152%

第八次：
平均等待时间：287.825909分钟
满意率：2.621212%

第九次：
平均等待时间：360.068106分钟
满意率：2.696970%

第十次：
平均等待时间：300.674924分钟
满意率：2.984848%
结论：
平均每个人等待电梯的时间是：‭316.5714393‬分钟‬‬
满意率是：‭2.719697‬%‬‬
改变程序中宏定义的ELEVATOR_NUMBER(电梯的数量)运行，直到运行结果满意率大于或者等于70，得出结论：
	当电梯数量大于70时，每次运行程序输出的满意率基本在70%以上，所以当电梯数量在70部时，平均满意率大于70%
四：误差分析：
Ⅰ每次程序运行时产生的数据都是随机的，最后的平均等待时间和满意率也是有波动的，这个理论误差可以通过增加实验次数来减小，根据大数定律，当抽样次数越来越大时，样本的平均值就会越来越趋近理论值。所以本次模拟采用多次实验求平均值的方法来减小误差。但是，由于试验次数越多，时间成本越高，所以这里只做了十次模拟。
Ⅱ本程序使用的生成满足泊松分布的随机数的方法是由数学家高纳德给出的，思路如下：
1，从Uniform(1)的均匀分布出发，用Inverse CDF 方法产生一系列独立的指数分布（参数为λ）随机数Xi~exp(λ)；
2，记Y=X1+ X2+X3+ …+ Xk。如果Y>t，则停止，输出k-1；若否，则继续生成Xk+1，直到Y>t为止；
3，重复过程2。
容易证明，输出的一系列整数k就满足服从参数为μ=λt的Poisson分布。
	本程序中，λ取11，实现这个功能的函数代码部分如下：
	int possion() {
	int k = 0;
	double u;
	long double p = 1.0;
	long double l = exp(-LAMBDA);
	while (p > l) {
		u = P_Random();//P_Random()函数功能是随机返回一个0到1之间的一个随机小数
		p *= u;
		k++;
	}
	return k-1;
}

但是，在测试中发现，生成的随机数列平均值约是10.99（1000000个随机数的平均值），小于理论值λ的值11，探索得出愿意是因为除法结果会舍弃超过double类型小数点后位数的部分。导致P_Random()返回的小数偏小，所以最后的随机数列平均值偏小。
double P_Random() {//生成一个0~1之间的随机数
	double f;
	f = (float)(rand());
	return f / RAND_MAX;//为了提升小数的精度，除数取rand()函数能生成的最大值
}
五．心得总结
这次大作业的代码量很大，模拟的过程也很繁琐，花了整整五天时间从早到晚来写这个作业，总代码行数也头一次超过了1000行。最开始的时候，本来打算模拟现实生活中电梯的运行的，但是在写到确定那一台电梯去接乘客的算法部分时候，发现情况实在是太繁琐，思路也很混乱，完全理不出头绪。在往这个方向写了一天之后，仍然毫无进展，于是选择了简化问题，假设进入写字楼只能从一楼进，出写字楼只去一楼，也就是说，不存在从二楼到五楼或者五楼到二楼这种情况。在简化问题后，很多之前繁琐的if…else if分支都能简化，思路也变的清晰了，最后也赶在截至时间之前写完了。
个人的收获也很多，头一次能够独立解决这样一个很贴近现实的问题，虽然中途好几次都很绝望，以至于想要草草应付了事，但最后还是坚持了下来，感觉到了解决问题的喜悦和自豪感。在调试的过程中，体会到规范代码风格的好处，如果我的代码能够更加规范一点，命名和分支更加清晰一些，那么调试的过程中也可以更加顺利，效率也会更高。所以以后写代码得更加注意这方面的习惯。写代码能锻炼意志力这个说法这次我深有体会，在毫无思路的时候，在脑子一团乱麻的时候，在焦急无助的时候，我都想过放弃，但是很庆幸，我咬牙写下来了。我心里十分渴望能写出来这个程序，甚至在写个作业期间，好几次梦见自己把它写出来了，最后也算美梦成真了吧。
