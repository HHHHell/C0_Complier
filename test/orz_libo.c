const char _o='o',_r='r',_z='z',_l='l',_i='i',_b='b';
const int xixi=19,haha=26,hugao=8,maogao=17;
const int history_process=+1;
const int minus1=-1;

int maxval[23333],minval[23333],sum[23333];
int data[2333];
int n;
int qsort_data[2333],m;

int partition(int low, int high){
    int key,flg;
    key = qsort_data[low];
    while(low<high){
    	flg=1;
    	if(low>=high) flg=0;
    	else;
    	if(qsort_data[high]<key) flg=0;
    	else;
        while(flg)  {
            high=high-1;
            if(low>=high) flg=0;
	    	else;
    		if(qsort_data[high]<key) flg=0;
        	else;
		}
        if(low<high) {
            qsort_data[low] = qsort_data[high];
            low=low+1;
        }
       	else;
    
        flg=0;
        if(low<high) if(qsort_data[low]<=key) flg=1;
    	else;
    	else;
        while(flg) {
            low=low+1;
            flg=0;
            if(low<high) if(qsort_data[low]<=key) flg=1;
	    	else;
	    	else;
        }
        if(low<high) {
            qsort_data[high] = qsort_data[low];
            high=high-1;
        }
    	else;
}
    qsort_data[low] = key;
    return(low);
}

void qsort(int start, int end){
    int pos;
    if (start<end){
        pos = partition(start, end);
        qsort(start,pos-1);
        qsort(pos+1,end);
    }
    else;
    return;
}

void calculate_e {
	const int maxn=233,max_iterate=233;
	int tmp[233],ans[233];
	int i,j,k,rem;
	
	i=0; while(i<maxn) {
		tmp[i]=0; ans[i]=0;
		i=i+1;
	}
	
	ans[0]=2; tmp[0]=1;
	i=1; while(i<=max_iterate) {
		rem=0;
		
		j=0; while(j<maxn) {
			rem=rem*10+tmp[j];
			tmp[j]=rem/i;
			rem=rem-rem/i*i;
		j=j+1; }
		
		j=maxn-1; while(j>=1) {
			ans[j]=ans[j]+tmp[j];
			if(ans[j]>10) {
				ans[j]=ans[j]-10;
				ans[j-1]=ans[j-1]+1;
			}
			else;
		j=j-1; }
	i=i+1; }
	
	printf(ans[0]);
	printf(".");
	i=1; while(i<10) {
		printf(ans[i]);
		i=i+1;
	}
} 

void weird_expressions_test {
	printf(--2++3);
	printf(-2+-5);
	printf(0);
	printf(+-1);
	printf(-+1);
	printf(--1);
	printf(++1);
	printf(-10-10); 
	
	printf(xixi,haha,0,hugao,maogao);
	printf("history_process",history_process,'s'); 
}

int min(int a,int b) {
	if(a<b) return (a);
	else;
	return (b);
}

int max(int a,int b) {
	if(a>b) return (a);
	else;
	return (b);
}

void build(int l,int r,int pos) {
	printf("Building segment (",l,", ",r,")...");
	
	if(l==r) {
		maxval[pos] = data[l];
		minval[pos] = data[l];
		sum[pos]=data[l];
		return;
	}
	else;
	
	build(l,(l+r)/2,pos*2);
	build((l+r)/2+1,r,pos*2+1); 
	
	maxval[pos] = max(maxval[pos*2],maxval[pos*2+1]);
	minval[pos] = min(minval[pos*2],minval[pos*2+1]);
	sum[pos] = sum[pos*2] + sum[pos*2+1];
}

int query(int l,int r,int type,int pos,int ll,int rr) {
	int flg,ans1,ans2;
	
	if(ll >= l) if(rr <= r) {
		if(type == 0) return(maxval[pos]);
		else;
		if(type == 1) return(minval[pos]);
		else;
		if(type == 2) return(sum[pos]);
		else;
		return(0);
	}
	else;
	else;
	flg=0;
	if(ll > r) flg=1;
	else;

	if(rr < l) flg=1;
	else;
	if(flg) {
		switch(type) {
			case 0: return(-2147483647);
			case 1: return (2147483647);
			case 2: return(0);
			default: return (0);
		}
	}
	else;
	
	switch(type) {
		case 0: return (max(query(l,r,type,pos*2,ll,(ll+rr)/2),query(l,r,type,pos*2+1,(ll+rr)/2+1,rr)));
		case 1: return (min(query(l,r,type,pos*2,ll,(ll+rr)/2),query(l,r,type,pos*2+1,(ll+rr)/2+1,rr)));
		case 2: return (query(l,r,type,pos*2,ll,(ll+rr)/2)+query(l,r,type,pos*2+1,(ll+rr)/2+1,rr));
	}
	return(0);
}

void echo {
	char x;
	while(1) {
		scanf(x);
		if(x=='0') return;
		else;
		printf(x);
		printf(x+2);
	}
}

void fibo {
	int tmp[4],ans[4],n,t[4];
	int i,j,k;
	
	while(1) {
		tmp[0]=0; tmp[1]=1; tmp[2]=1; tmp[3]=tmp[2];
		ans[0]=0; ans[1]=1; ans[2]=1; ans[3]=0;
		
		printf("Please input n, and the program will work out the n-th element of the fibonacci sequence:");
		printf("Input 0 to quit this test.");
		scanf(n);
		if(n==0) return;
		else;
		
		while(n) {
			if(n-n/2*2) {
				i=0; while(i<2) {
					j=0; while(j<2) {
						t[i*2+j]=0;
						k=0; while(k<2) {
							t[i*2+j]=t[i*2+j]+ans[i*2+k]*tmp[k*2+j];
							t[i*2+j]=t[i*2+j]-t[i*2+j]/10000*10000;
							k=k+1;
						}
						j=j+1;
					}
					i=i+1;
				}
				i=0; while(i<4) {
					ans[i]=t[i];
					i=i+1;
				}
			}
			else;
			
			i=0; while(i<2) {
				j=0; while(j<2) {
					t[i*2+j]=0;
					k=0; while(k<2) {
						t[i*2+j]=t[i*2+j]+tmp[i*2+k]*tmp[k*2+j];
						t[i*2+j]=t[i*2+j]-t[i*2+j]/10000*10000;
						k=k+1;
					}
					j=j+1;
				}
				i=i+1;
			}
			i=0; while(i<4) {
				tmp[i]=t[i];
				i=i+1;
			}
			n=n/2;
		}
		printf("The result is: ", ans[1]);
	}
}

int gcd(int a,int b) {
	if(b==0) return(a);
	else;
	return(gcd(b,a-a/b*b));
}

void main() {
	int i,j;
	int a,b,flg,x;
	int l,r,type;
	
	i=0;
	printf("The program first orzs libo to gain some rp and increase the probability of passing the test.");
	while(i<10) {
		printf("orzlibo ", i);
		i=i+1;
	}
	printf(_o,_r,_z,_l,_i,_b,_o);
	
	flg=1;
	while(flg) {
		printf("please input 2 integers, and the program will work out their gcd.");
		printf("input 2 zeros to quit the loop.");
		scanf(a,b);
		if(a==0) if(b==0) flg=0;
		else;
		else;
		if(flg) printf("the gcd is: ",gcd(a,b));
		else;
	}
	
	printf("Now, let's do some math problems for fun.");
	fibo;
	
	printf("Well, let's run another segment tree test to verify its correctness.");
	printf("Please input the length of the sequence:");
	scanf(n);
	
	i=1; while(i<=n) {
		printf("Please input the ", i, "-th number: ");
		scanf(x);
		data[i]=x;
		i=i+1;
	}
	
	printf("The numbers you just inputed are:");
	i=1; while(i<=n) {
		printf("The ", i, "-th: ",data[i]);
		i=i+1;
	}
	
	printf("Building segment tree...");
	build(1,n,1);
	printf("The tree is succesfully built! Now it can respond to your query requests.");
	flg=1;
	while(flg) {
		printf("Please input 3 numbers, l, r, and type, type=-1 means exit:");
		scanf(l,r,type);
		if(type<0) flg=0;
		else;
		if(type>=3) flg=0;
		else;
		if(flg) {
			printf("Your query result is ",query(l,r,type,1,1,n));
		}
		else;
	}
	
	printf("Echo testing...");
	echo;
	
	weird_expressions_test();
	
	calculate_e;
	
	printf("Quick sort test:");
	printf("Please input the number of integers:");
	scanf(m);
	i=1; while(i<=m) {
		printf("Please input the ",i,"-th number:");
		scanf(x);
		qsort_data[i]=x;
		i=i+1;
	}
	qsort(1,m);
	printf("The result after sorting is:");
	i=1; while(i<=m) {
		printf(qsort_data[i]);
		i=i+1;
	}
}
