import { Box, IconButton, useTheme, Link } from "@mui/material";
import { useContext } from "react";
import { ColorModeContext } from "../../theme";
import LightModeOutlinedIcon from "@mui/icons-material/LightModeOutlined";
import DarkModeOutlinedIcon from "@mui/icons-material/DarkModeOutlined";
import GitHubIcon from '@mui/icons-material/GitHub';
// import PersonOutlinedIcon from "@mui/icons-material/PersonOutlined";
import SearchIcon from "@mui/icons-material/Search";

import LogoUnB from "../../assets/logo-UnB.png";

const Topbar = () => {
  const theme = useTheme();
  // const colors = tokens(theme.palette.mode);
  const colorMode = useContext(ColorModeContext);

  return (
    <Box display="flex" justifyContent="space-between" p={2}>
      {/* SEARCH BAR */}
      <Box
        display="flex"
        borderRadius="3px"
      >
        <IconButton type="button" sx={{ p: 1 }}>
          <SearchIcon />
        </IconButton>
        <img src={LogoUnB} style={{ width: '100px', height: '50px', marginLeft: '10px'}} alt="Logo UnB" />
      </Box>

      {/* ICONS */}
      <Box display="flex">
        <IconButton onClick={colorMode.toggleColorMode}>
          {theme.palette.mode === "dark" ? (
            <DarkModeOutlinedIcon />
          ) : (
            <LightModeOutlinedIcon />
          )}
        </IconButton>
        <Link href="https://github.com/izabellaalves/pi1-juliana-grupo2" target="_blank">
          <IconButton>
            <GitHubIcon />
          </IconButton>
        </Link>
      </Box>
    </Box>
  );
};

export default Topbar;
