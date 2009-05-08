#include "py_colpack.hpp"

bp::list	wrapped_jac_pat(short tape_tag, bpn::array &bpn_x,bpn::array &bpn_options){
	int tape_stats[STAT_SIZE];
	tapestats(tape_tag, tape_stats);
	npy_intp N = tape_stats[NUM_INDEPENDENTS];
	npy_intp M = tape_stats[NUM_DEPENDENTS];

	double* x = (double*) nu::data(bpn_x);
	int* options  = (int*) nu::data(bpn_options);
	unsigned int* JP[M];

	jac_pat(tape_tag, M, N, x, JP, options);

	bp::list ret_JP;

	for(int m = 0; m != M; ++m){
		bp::list tmp;
		ret_JP.append(tmp);
		for(int c = 1; c <= JP[m][0]; ++c){
			bp::list tmp =  boost::python::extract<boost::python::list>(ret_JP[m]);
			tmp.append(JP[m][c]);
		}
	}

	return ret_JP;
}


bp::list	wrapped_sparse_jac_no_repeat(short tape_tag, bpn::array &bpn_x, bpn::array &bpn_options){
	int tape_stats[STAT_SIZE];
	tapestats(tape_tag, tape_stats);
	npy_intp N = tape_stats[NUM_INDEPENDENTS];
	npy_intp M = tape_stats[NUM_DEPENDENTS];

	double* x = (double*) nu::data(bpn_x);
	int* options  = (int*) nu::data(bpn_options);
	int nnz=-1;
	unsigned int *rind;
	unsigned int *cind;
	double   *values;
	sparse_jac(tape_tag, M, N, 0, x, &nnz, &rind, &cind, &values, options);

	npy_intp ret_nnz = static_cast<npy_intp>(nnz);
	bp::object bp_rind   ( bp::handle<>(PyArray_SimpleNewFromData(1, &ret_nnz, PyArray_INT, (char*) rind )));
	bp::object bp_cind   ( bp::handle<>(PyArray_SimpleNewFromData(1, &ret_nnz, PyArray_INT, (char*) cind )));
	bp::object bp_values ( bp::handle<>(PyArray_SimpleNewFromData(1, &ret_nnz, PyArray_DOUBLE, (char*) values )));

	bpn::array ret_rind   = boost::python::extract<boost::python::numeric::array>(bp_rind);
	bpn::array ret_cind   = boost::python::extract<boost::python::numeric::array>(bp_cind);
	bpn::array ret_values = boost::python::extract<boost::python::numeric::array>(bp_values);

	bp::list retvals;
	retvals.append(ret_nnz);
	retvals.append(ret_rind);
	retvals.append(ret_cind);
	retvals.append(ret_values);

	return retvals;

}

bp::list	wrapped_sparse_jac_repeat(short tape_tag, bpn::array &bpn_x, npy_intp nnz, bpn::array &bpn_rind, bpn::array &bpn_cind, bpn::array &bpn_values){
	int tape_stats[STAT_SIZE];
	tapestats(tape_tag, tape_stats);
	npy_intp N = tape_stats[NUM_INDEPENDENTS];
	npy_intp M = tape_stats[NUM_DEPENDENTS];

	double* x          = (double*)   nu::data(bpn_x);
	unsigned int* rind       = (unsigned int*)   nu::data(bpn_rind);
	unsigned int* cind       = (unsigned int*)   nu::data(bpn_cind);
	double   *values   = (double*)   nu::data(bpn_values);
	int options[4]={0,0,0,0};
	int tmp_nnz = static_cast<int>(nnz);

	sparse_jac(tape_tag, M, N, 1, x, &tmp_nnz, &rind, &cind, &values, options);

	bp::object bp_rind   ( bp::handle<>(PyArray_SimpleNewFromData(1, &nnz, PyArray_INT, (char*) rind )));
	bp::object bp_cind   ( bp::handle<>(PyArray_SimpleNewFromData(1, &nnz, PyArray_INT, (char*) cind )));
	bp::object bp_values ( bp::handle<>(PyArray_SimpleNewFromData(1, &nnz, PyArray_DOUBLE, (char*) values )));

	bpn::array ret_rind   = boost::python::extract<boost::python::numeric::array>(bp_rind);
	bpn::array ret_cind   = boost::python::extract<boost::python::numeric::array>(bp_cind);
	bpn::array ret_values = boost::python::extract<boost::python::numeric::array>(bp_values);

	bp::list retvals;
	retvals.append(nnz);
	retvals.append(ret_rind);
	retvals.append(ret_cind);
	retvals.append(ret_values);

	return retvals;

}
