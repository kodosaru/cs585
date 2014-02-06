% Return all raw moments needed to calulate central moments

function [m00 m10 m01 m20 m02 m21 m12 m30 m03]=spatial_moments(A)
	[rows cols]=size(A);
	m00=0;
	m10=0;
	m01=0;
	m20=0;
	m02=0;
	m21=0;
	m12=0;
	m30=0;
	m03=0;
	for x=0:cols-1
		for y=0:rows-1
			m00=m00+uint64(A(y+1,x+1));
			m10=m10+x*uint64(A(y+1,x+1));
			m01=m01+y*uint64(A(y+1,x+1));
			m20=m20+x^2*uint64(A(y+1,x+1));
			m02=m02+y^2*uint64(A(y+1,x+1));
			m21=m21+x^2*y*uint64(A(y+1,x+1));
			m12=m12+x*y^2*uint64(A(y+1,x+1));
			m30=m30+x^3*uint64(A(y+1,x+1));
			m03=m03+y^3*uint64(A(y+1,x+1));
		end 
	end
end
