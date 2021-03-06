/*
    Gri - A language for scientific graphics programming
    Copyright (C) 2008 Daniel Kelley

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "gr.hh"
#include "private.hh"
#include "extern.hh"
#include "gr_coll.hh"
#include "DataFile.hh"

extern bool     _ignore_error;
bool            skip_forwardCmd(int n);
bool            skip_backwardCmd(int n);

// skipCmd() -- control skipping in data file
bool
skipCmd()
{
	double           tmp;
	bool            old_ignore_error = _ignore_error;
	if (_dataFILE.back().get_type() == DataFile::from_cmdfile) {
		err("`skip' ignored: no data file open");
		return false;
	}
	if (_dataFILE.back().get_type() == DataFile::bin_netcdf) {
		warning("`skip' ignored: cannot do this for netCDF file");
		return false;
	}
	if (_nword == 1)
		return skip_forwardCmd(1);
	_ignore_error = true;
	if (getdnum(_word[1], &tmp)) {
		skip_forwardCmd(int(floor(0.5 + tmp)));
		_ignore_error = old_ignore_error;
		return true;
	}
	if (!strcmp(_word[1], "forward")) {
		if (_nword == 2)
			return skip_forwardCmd(1);
		else {
			double           tmp;
			if (!getdnum(_word[2], &tmp)) {
				err("invalid # to 'skip forward'");
				_ignore_error = old_ignore_error;
				return false;
			}
			return skip_forwardCmd(int(floor(0.5 + tmp)));
		}
	} else if (!strcmp(_word[1], "backward")) {
		if (_nword == 2)
			return skip_backwardCmd(1);
		else {
			if (!getdnum(_word[2], &tmp)) {
				err("invalid # to 'skip backward'");
				return false;
			}
			return skip_backwardCmd(int(floor(0.5 + tmp)));
		}
	} else {
		err("Unknown 'skip' parameter;\nvalid choices: forward/backward");
		_ignore_error = old_ignore_error;
		return true;
	}
}

bool
skip_forwardCmd(int n)
{
	Require(n >= 0,
		err("Require positive (or zero) number of lines to skip"));
	if (_dataFILE.back().get_type() == DataFile::from_cmdfile) {
		warning("Cannot `skip' when data are embedded in command-file");
		return true;
	}
	while (n-- > 0) {
		if (_dataFILE.back().get_type() != DataFile::ascii) {
			// type=0 ascii, others binary
			unsigned char   tmpB;
			if (1 != fread((char *) & tmpB, sizeof(tmpB), 1, _dataFILE.back().get_fp())) {
				warning("I/O error while skip forward in binary file.");
				return false;
			}
			if (feof(_dataFILE.back().get_fp())) {
				set_eof_flag_on_data_file();
				warning("`skip forward' at end-of-file on file `\\",
					_dataFILE.back().get_name(),  "'", "\\");
				break;
			}
		} else {
			// Ascii file
			GriString inLine(128); // Start short
			if (inLine.line_from_FILE(_dataFILE.back().get_fp())) {
				set_eof_flag_on_data_file();
				warning("`skip forward' at end-of-file on file `\\",
					_dataFILE.back().get_name(),  "'", "\\");
				return true;
			}
			_dataFILE.back().increment_line();
		}
	}
	return true;
}

bool
skip_backwardCmd(int n)
{
	Require(n >= 0,
		err("Require positive (or zero) number of lines to skip"));
	if (_dataFILE.back().get_type() == DataFile::from_cmdfile) {
		warning("Cannot `skip' when data are embedded in command-file");
		return true;
	}
	if (_dataFILE.back().get_type() != DataFile::ascii) {
		warning("Can't `skip forward' in binary files");
		return false;
	}
	unsigned int present_line = _dataFILE.back().get_line();
	//printf("back skip %d was at present_line=%d\n",n,present_line);
	rewind(_dataFILE.back().get_fp());
	_dataFILE.back().set_line(0);
	clearerr(_dataFILE.back().get_fp());
	if (n > int(present_line)) {
		warning("Too few lines to skip that far; instead, rewinding the file.");
		return true;
	}
	GriString inLine(128); // Start short
	int num_to_advance = present_line - n;
	printf("*** num_to_advance %d\n",num_to_advance);
	for (int l = 0; l < num_to_advance; l++) {
		//printf(" skip\n");
		if (inLine.line_from_FILE(_dataFILE.back().get_fp())) {
			set_eof_flag_on_data_file();
			warning("`skip back' hit end-of-file on file `\\",
				_dataFILE.back().get_name(), "'", "\\");
			break;
		}
		_dataFILE.back().increment_line();
	}
	return true;
}
